#include "knapsack.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

int recursiveSolve(int capacity, const std::vector<Item>& items, int n) {
    if (n == 0 || capacity == 0)
        return 0;

    // если вес n-го предмета больше вместимости, пропускаем его
    if (items[n - 1].weight > capacity)
        return recursiveSolve(capacity, items, n - 1);

    // иначе возвращаем максимум из двух вариантов:
    // (1) взять предмет + результат для оставшегося веса
    // (2) не брать предмет
    return std::max(
        items[n - 1].value + recursiveSolve(capacity - items[n - 1].weight, items, n - 1),
        recursiveSolve(capacity, items, n - 1)
    );
}

// --- Полный перебор (Brute Force) O(2^N) ---
Result solveBruteForce(int capacity, const std::vector<Item>& items) {
    // в чистой рекурсии сложно восстановить список предметов без доп. памяти.
    // здесь мы возвращаем только макс. стоимость для демонстрации алгоритма.
    int maxVal = recursiveSolve(capacity, items, items.size());
    return { maxVal, {}, "Brute Force (Recursion)" };
}

// --- Динамическое программирование (DP) O(N * Capacity) ---
Result solveDP(int capacity, const std::vector<Item>& items) {
    int n = items.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    // заполнение таблицы
    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= capacity; ++w) {
            if (items[i - 1].weight <= w) {
                dp[i][w] = std::max(items[i - 1].value + dp[i - 1][w - items[i - 1].weight], dp[i - 1][w]);
            }
            else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // восстановление ответа (какие предметы взяли)
    std::vector<Item> selected;
    int res = dp[n][capacity];
    int w = capacity;
    for (int i = n; i > 0 && res > 0; i--) {
        // если значение пришло сверху (dp[i-1][w]), значит предмет не брали
        if (res != dp[i - 1][w]) {
            selected.push_back(items[i - 1]);
            res -= items[i - 1].value;
            w -= items[i - 1].weight;
        }
    }

    return { dp[n][capacity], selected, "Dynamic Programming" };
}

// --- Жадный алгоритм (Greedy) O(N log N) - из-за сортировки ---
// эвристика: берем предметы с максимальным соотношением Цена/Вес
Result solveGreedy(int capacity, std::vector<Item> items) {
    // сортируем по убыванию удельной стоимости (value / weight)
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        double r1 = (double)a.value / a.weight;
        double r2 = (double)b.value / b.weight;
        return r1 > r2;
        });

    int currentWeight = 0;
    int totalValue = 0;
    std::vector<Item> selected;

    for (const auto& item : items) {
        if (currentWeight + item.weight <= capacity) {
            currentWeight += item.weight;
            totalValue += item.value;
            selected.push_back(item);
        }
    }

    return { totalValue, selected, "Greedy Algorithm (Heuristic)" };
}
