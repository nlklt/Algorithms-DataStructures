#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

// Вспомогательная ф-ия для вывода вектора
void printVector(const std::vector<int>& v)
{
    for (int x : v) std::cout << x << " ";
}

// Вспомогательная ф-ия генерации случайного вектора
std::vector<int> generateRandomVector(int size)
{
    std::vector<int> nums(size);
    // генератор случайных чисел
    std::mt19937 rng(42);    // 42 - фиксированное зерно(seed)
    std::uniform_int_distribution<int> dist(0, 100000);

    for (int& x : nums)
    {
        x = dist(rng);
    }
    return nums;
}

void printTime(std::chrono::high_resolution_clock::duration duration)
{
    using namespace std::chrono;

    auto mins = duration_cast<minutes>(duration);
    duration -= mins;

    auto secs = duration_cast<seconds>(duration);
    duration -= secs;

    auto ms = duration_cast<milliseconds>(duration);
    duration -= ms;

    auto us = duration_cast<microseconds>(duration);

    std::cout << mins.count() << "m " << secs.count() << "s "
        << ms.count() << "ms " << us.count() << "mcs";
}
