#include "threadpool.h"
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main() {
    auto& pool = ThreadPool::getInstance();

    auto f1 = pool.commit([] { std::cout << "[A] tid = " << std::this_thread::get_id() << '\n'; });
    f1.get();

    auto f2 = pool.commit(
        [](int a, int b) {
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
            return a + b;
        },
        10, 32);
    std::cout << "sum = " << f2.get() << '\n';

    std::vector<std::future<int>> fs;
    for (int i = 1; i <= 5; ++i) {
        fs.emplace_back(pool.commit([i] { return i * i; }));
    }
    std::cout << "squares: ";
    for (auto& f : fs) {
        std::cout << f.get() << ' ';
    }
    std::cout << '\n';

    auto f3 = pool.commit([]() -> int { throw std::runtime_error("boom"); });
    try {
        (void)f3.get();
    } catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << '\n';
    }

    std::cout << "idle threads = " << pool.idleThreadCount() << '\n';
    return 0;
}