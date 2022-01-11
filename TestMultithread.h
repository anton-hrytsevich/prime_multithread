// TestMultithread.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>
#include <future>
#include <chrono>
#include <atomic>

class ThreadPool final {

private:
    std::vector<std::thread> _threads;

    std::atomic<bool>        _stopping = false;
    std::atomic<bool>        _is_not_prime = false;
    uint64_t                 _test_number = 0;

    bool prime_test(uint64_t start, uint64_t end) {
        while (start++ < end && !_stopping) {
            if ((_test_number % start) == 0) { // is prime?
                _is_not_prime = true;
                _stopping = true;
                return true;
            }
        }
        return false;
    }

public:
    explicit ThreadPool(uint64_t number) :
        _test_number(number) {
    }

    ~ThreadPool() {
        wait();
    }

    ThreadPool(const ThreadPool& c) = delete;
    ThreadPool& operator=(const ThreadPool& c) = delete;

    void add_thread(uint64_t left_bound, uint64_t right_bound) {
        if (_stopping) return;

        _threads.emplace_back([=] {prime_test(left_bound, right_bound); });
    }

    void wait() noexcept {
        for (auto& thread : _threads)
            if (thread.joinable()) thread.join();
    }

    bool result() noexcept  { return _is_not_prime; }

};

class SpeedMessuare {
public:
    SpeedMessuare() {
        auto start_point = std::chrono::high_resolution_clock::now();
        auto a = meassure_unit();
        auto end_point = std::chrono::high_resolution_clock::now();
        auto duration = end_point - start_point;
        _standart_duration = static_cast<double>(duration.count());
    }

    ~SpeedMessuare() {}
    SpeedMessuare(const SpeedMessuare& c) = delete;
    SpeedMessuare& operator=(const SpeedMessuare& c) = delete;

    uint32_t meassure() {
        auto start_point = std::chrono::high_resolution_clock::now();
        auto a = meassure_unit();
        auto end_point = std::chrono::high_resolution_clock::now();
        double duration = (end_point - start_point).count();
        
        auto delta = (duration - _standart_duration);
        auto midl = (delta / _standart_duration) * 100;
        return static_cast<uint32_t>(midl);
    }

private:
    uint32_t meassure_unit() {
        volatile uint32_t i = 0;
        while (i < 0xFFFFFFF) i++;
        return i;
    }

    double _standart_duration;
};