#include "knapsack.h" 
#include "utils.h"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

int main() {
    setlocale(LC_ALL, "");

    // задаем вместимость рюкзака
    int N = 40;
    std::cout << "backpack capacity: " << N << "\n\n";

    // инициализируем вектор предметов
    std::vector<Item> items = {
        { "Notebook",   10, 60 },  
        { "Projector",  20, 100 },
        { "Camera",     30, 120 },  
        { "Clocks",     5,  30 }       
    };

    std::cout << "available items:\n";
    std::cout << "----------------------------------------\n";
    for (const auto& item : items) {
        std::cout << "- " << std::setw(10) << std::left << item.name;
        std::cout << "| weight: " << std::setw(4) << std::left << item.weight
            << "| cost: " << item.value << "\n";
    }
    std::cout << "----------------------------------------\n";

    // 1. Brute Force (Полный перебор)
    //printResult(solveBruteForce(N, items));

    // 2. Dynamic Programming (Точное оптимальное решение)
    printResult(solveDP(N, items));

    // 3. Greedy (Жадный алгоритм)
    // жадный алгоритм здесь может ошибиться. 
    // он возьмет "Camera" (самый выгодный по соотношению) и 'Notebook'.
    // сумма = 160, Вес = 40. Осталось 0 места.
    //printResult(solveGreedy(N, items));

    return 0;
}