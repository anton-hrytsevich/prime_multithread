// TestMultithread.cpp : Defines the entry point for the application.
//

#include "TestMultithread.h"
#include <condition_variable>


int main() {
    using namespace std;
    using namespace std::chrono_literals;
    condition_variable variable;
    SpeedMessuare metr;
    uint64_t test_number = 0x3235434f34534FD0;
    ThreadPool pool{ test_number };

    for (auto i = 2; i < test_number; ) {
        uint64_t left_bound = i;
        uint64_t right_bound = (i + 0xFF) > test_number ? (test_number - 1) : (i + 0xFF); 
        pool.add_thread(i, i + 0xFF);

        uint32_t m1 = metr.meassure();
        cout << m1 << endl;

        //if (m1 > 200 ) std::this_thread::sleep_for(2000ms);
        if (pool.result() == true) break;
    }
    pool.wait();

    if (pool.result()) {
        cout << "Number: "<< test_number << " is NOT prime." << endl;
    }
    else {
        cout << "Number: " << test_number << " is prime." << endl;
    }

    return 0;
}
