#include "knapsack.h" 
#include "utils.h"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

int main() {
    setlocale(LC_ALL, "");

    // задаем вместимость рюкзака
    int N = 50;
    std::cout << "backpack capacity: " << N << "\n" << std::endl;

    // инициализируем вектор предметов тестовыми данными
    std::vector<Item> items = {
        { "Notebook", 10, 60 },  
        { "Projector", 20, 100 },
        { "Camera", 30, 120 },  
        { "Clocks", 5, 30 }       
    };

    std::cout << "available items:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    for (const auto& item : items) {
        std::cout << "- " << std::setw(10) << std::left << item.name;
        std::cout << "| weight: " << std::setw(4) << std::left << item.weight
            << "| cost: " << item.value << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    // 1. Brute Force (Полный перебор)
    printResult(solveBruteForce(N, items));

    // 2. Dynamic Programming (Точное оптимальное решение)
    printResult(solveDP(N, items));

    // 3. Greedy (Жадный алгоритм)
    // жадный алгоритм здесь может ошибиться. 
    // он возьмет 'Ноутбук' (самый выгодный по соотношению) и 'Проектор'.
    // сумма = 160, Вес = 30. Осталось 20 места, 'Камера' (30) не лезет.
    // а DP возьмет 'Проектор' и 'Камеру'. Сумма = 220.
    printResult(solveGreedy(N, items));

    return 0;
}