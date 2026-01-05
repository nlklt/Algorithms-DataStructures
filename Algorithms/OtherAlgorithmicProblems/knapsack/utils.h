#pragma once
#include "knapsack.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

// Вспомогательная ф-ия для вывода
void printResult(const Result& res) {
    std::cout << "\n--- " << res.methodName << " ---" << "\n";
    std::cout << "maximum cost: " << res.maxValue << "\n";
    if (!res.selectedItems.empty()) {
        std::cout << "selected items:\n";
        for (const auto& item : res.selectedItems) {
            std::cout << " - " << std::setw(10) << std::left << item.name << 
                " (weight: " << std::setw(3) << std::left << item.weight << 
                "cost: " << item.value << ")\n";
        }
    }
    else {
        std::cout << "list of items is not being restored\n";
    }
}