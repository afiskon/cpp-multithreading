/* vim: set ai et ts=4 sw=4: */

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

static std::atomic_int atomic_counter(0);
static const int MAX_COUNTER_VAL = 100;

std::mutex io_mtx;

void thread_proc(int tnum) {
    for(;;) {
        {
            int ctr_val = ++atomic_counter;
            if(ctr_val >= MAX_COUNTER_VAL)
                break;

            {
                std::lock_guard<std::mutex> lock(io_mtx);
                std::cout << "Thread " << tnum << ": counter = " << ctr_val << std::endl;
            }
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
