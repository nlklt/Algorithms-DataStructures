#pragma once
#include <vector>
#include <string>

// Структура для предмета
struct Item {
    std::string name;
    int weight;
    int value;
};

// Структура для возврата результата
struct Result {
    int maxValue;
    std::vector<Item> selectedItems;
    std::string methodName;
};

// --- Полный перебор (Brute Force) O(2^N) ---
int recursiveSolve(int capacity, const std::vector<Item>& items, int n);
Result solveBruteForce(int capacity, const std::vector<Item>& items);

// --- Динамическое программирование (DP) O(N * Capacity) ---
Result solveDP(int capacity, const std::vector<Item>&items);

// --- Жадный алгоритм (Greedy) O(N log N) - из-за сортировки ---
// эвристика: берем предметы с максимальным соотношением Цена/Вес
Result solveGreedy(int capacity, std::vector<Item> items);
