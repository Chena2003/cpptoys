#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <utility>

class ThreadPool {
  public:
    using Tasks = std::packaged_task<void()>;

    static ThreadPool& getInstance(int thread_nums = 5) {
        static ThreadPool instance(thread_nums);
        return instance;
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ~ThreadPool() { stop(); }

    template <class F, class... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using RetType = decltype(f(args...));
        if (stop_.load())
            return std::future<RetType>{};

        // std::bind 将有参数的函数绑定成一个无参数函数
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<RetType> ret = task->get_future();
        {
            std::lock_guard<std::mutex> cv_mt(cv_mt_);
            tasks_.emplace([task] { (*task)(); });
        }

        cv_lock_.notify_one();
        return ret;
    }

    int idleThreadCount() { return thread_nums_; }

  private:
    ThreadPool(int thread_nums) : thread_nums_(thread_nums), stop_(false) {
        thread_nums_ = thread_nums_ < 1 ? 1 : thread_nums;
        start();
    };

    void start() {
        for (int i = 0; i < thread_nums_; ++i) {
            pool_.emplace_back([this]() {
                while (true) {
                    Tasks task;
                    {
                        std::unique_lock<std::mutex> cv_mt(cv_mt_);
                        this->cv_lock_.wait(
                            cv_mt, [this] { return this->stop_.load() || !this->tasks_.empty(); });

                        if (stop_.load() && tasks_.empty()) {
                            return;
                        }

                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    --this->thread_nums_;
                    task();
                    ++this->thread_nums_;
                }
            });
        }
    }

    void stop() {
        stop_.store(true);
        cv_lock_.notify_all();
        for (auto& td : pool_) {
            if (td.joinable()) {
                std::cout << "join thread " << td.get_id() << std::endl;
                td.join();
            }
        }
    }

  private:
    std::mutex cv_mt_;
    std::condition_variable cv_lock_;
    std::atomic<bool> stop_;
    std::atomic<int> thread_nums_;
    std::queue<Tasks> tasks_;
    std::vector<std::thread> pool_;
};

#endif // __THREAD_POOL_H__