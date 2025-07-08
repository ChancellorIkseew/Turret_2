#include <chrono>
#include <thread>

namespace util {
    inline void sleep(const int timeInMilliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeInMilliseconds));
    }
}
