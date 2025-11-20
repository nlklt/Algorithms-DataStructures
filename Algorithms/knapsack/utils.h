#pragma once
#include "knapsack.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

// Вспомогательная ф-ия для вывода
void printResult(const Result& res) {
    std::cout << "\n--- " << res.methodName << " ---" << std::endl;
    std::cout << "maximum cost: " << res.maxValue << std::endl;
    if (!res.selectedItems.empty()) {
        std::cout << "selected items:" << std::endl;
        for (const auto& item : res.selectedItems) {
            std::cout << " - " << std::setw(10) << std::left << item.name << " (weight: " << item.weight << ", cost: " << item.value << ")" << std::endl;
        }
    }
    else if (res.methodName.find("Recursion") != std::string::npos) {
        std::cout << "list of items is not being restored" << std::endl;
    }
}