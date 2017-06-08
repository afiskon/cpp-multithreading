/* vim: set ai et ts=4 sw=4: */

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex mtx;
static int counter = 0;
static const int MAX_COUNTER_VAL = 100;

void thread_proc(int tnum) {
    for(;;) {
        {
            // std::scoped_lock lock(mtx); // not available in Clang 4.0
            std::lock_guard<std::mutex> lock(mtx);
            if(counter == MAX_COUNTER_VAL)
                break;
            int ctr_val = ++counter;
            std::cout << "Thread " << tnum << ": counter = " << ctr_val << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    std::vector<std::thread> threads;
    for(int i = 0; i < 10; i++) {
        std::thread thr(thread_proc, i);
        threads.emplace_back(std::move(thr));
    }

    // can't use const auto& here since .join() is not marked const
    for(auto& thr : threads) {
        thr.join();
    }

    std::cout << "Done!" << std::endl;
    return 0;
}
