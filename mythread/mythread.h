/*
** MyThread - A simple thread wrapper class
** 可以使用 jthread(c++20) 替代
*/

#include <functional>
#include <thread>

namespace Chena {

class MyThread {

  public:
    MyThread() {};
    template <typename Function, typename... Args> explicit MyThread(Function&& f, Args&&... args) {
        thread_ = std::thread(std::forward<Function>(f), std::forward<Args>(args)...);
    }

    MyThread(const MyThread&) = delete;
    MyThread& operator=(const MyThread&) = delete;

    MyThread(MyThread&& other) noexcept : thread_(std::move(other.thread_)) {}

    ~MyThread() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

  private:
    std::thread thread_;
};

} // namespace Chena