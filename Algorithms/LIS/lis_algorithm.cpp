#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <sstream>
#include <Windows.h>

// ==========================================
// УТИЛИТЫ КОНСОЛИ
// ==========================================

enum Color {
    DEFAULT = 7,
    GREEN = 10,   // i (текущий элемент)
    CYAN = 11,    // j или элемент Tails для сравнения
    YELLOW = 14,  // UPDATE (момент записи)
    RED = 12,     // FAIL (условие не выполнено)
    GRAY = 8      // Для рамок
};

inline void setColor(int color) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, color);
}

inline void resetCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hOut, coord);
}

inline void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

// ==========================================
// ОТРИСОВКА N^2 И ОБЩАЯ
// ==========================================

// Универсальная функция отрисовки строки
// idx_i - главный индекс (GREEN/YELLOW)
// idx_j - вспомогательный индекс (CYAN)
inline void drawRow(std::string name, const std::vector<int>& a, int n,
    int idx_i, int idx_j, bool isUpdateEvent)
{
    const int CELL_WIDTH = 4;
    int frameColor = GRAY;

    // --- ВЕРХНЯЯ РАМКА ---
    setColor(frameColor);
    std::cout << "        " << u8"┌";
    for (int k = 0; k < n; ++k) {
        for (int w = 0; w < CELL_WIDTH - 1; ++w) std::cout << u8"─";
        if (k < n - 1) std::cout << u8"┬";
    }
    std::cout << u8"┐\n";

    // --- ДАННЫЕ ---
    setColor(DEFAULT);
    std::cout << std::setw(7) << std::left << (name + ": ");
    setColor(frameColor);
    std::cout << u8"│";

    for (int k = 0; k < n; ++k) {
        int currentColor = DEFAULT;

        if (k == idx_i) {
            currentColor = isUpdateEvent ? YELLOW : GREEN;
        }
        else if (k == idx_j) {
            currentColor = CYAN;
        }

        std::string valStr = std::to_string(a[k]);
        std::string cell = "";
        int spaces = (CELL_WIDTH - 1) - (int)valStr.length();
        if (spaces < 0) spaces = 0;
        for (int s = 0; s < spaces; ++s) cell += " ";
        cell += valStr;

        setColor(currentColor);
        std::cout << cell;

        setColor(frameColor);
        std::cout << u8"│";
    }
    std::cout << "\n";

    // --- НИЖНЯЯ РАМКА ---
    setColor(frameColor);
    std::cout << "        " << u8"└";
    for (int k = 0; k < n; ++k) {
        for (int w = 0; w < CELL_WIDTH - 1; ++w) std::cout << u8"─";
        if (k < n - 1) std::cout << u8"┴";
    }
    std::cout << u8"┘\n";
}

// === PRINT STATE O(N^2) ===
inline void printState(const std::vector<int>& nums,
    const std::vector<int>& dp,
    const std::vector<int>& parent,
    int idx_i, int idx_j,
    std::string statusMsg,
    bool isUpdateEvent = false)
{
    resetCursor();

    setColor(DEFAULT); std::cout << "Status O(N^2): ";
    if (isUpdateEvent) setColor(YELLOW);
    else if (statusMsg.find("Skip") != std::string::npos) setColor(RED);
    else setColor(DEFAULT);

    std::cout << std::setw(50) << std::left << statusMsg << "\n\n";

    int n = (int)nums.size();

    drawRow("nums", nums, n, idx_i, idx_j, false);
    drawRow("dp", dp, n, idx_i, idx_j, isUpdateEvent);
    drawRow("parent", parent, n, idx_i, idx_j, isUpdateEvent);

    std::cout << "\n";
    setColor(GREEN);  std::cout << " i "; setColor(DEFAULT); std::cout << "= current end";
    setColor(CYAN);   std::cout << "   j "; setColor(DEFAULT); std::cout << "= previous";
    setColor(YELLOW); std::cout << "   Update!";
    setColor(DEFAULT); std::cout << "       \n";
}

// === PRINT STATE O(N Log N) ===
inline void printStateNLogN(const std::vector<int>& nums,
    const std::vector<int>& parent,
    const std::vector<int>& tails_indices,
    int idx_i,
    int idx_tail_update,
    std::string statusMsg,
    bool isUpdateEvent = false)
{
    resetCursor();

    setColor(DEFAULT); std::cout << "Status O(N Log N): ";
    if (isUpdateEvent) setColor(YELLOW);
    else if (statusMsg.find("NO") != std::string::npos) setColor(RED);
    else setColor(DEFAULT);

    std::cout << std::setw(50) << std::left << statusMsg << "\n\n";

    int n = (int)nums.size();

    // 1. Рисуем NUMS
    drawRow("nums", nums, n, idx_i, -1, false);

    // 2. Рисуем PARENT
    drawRow("parent", parent, n, idx_i, -1, isUpdateEvent);

    std::cout << "\n";

    // 3. Рисуем TAILS.
    std::vector<int> tailsValues;
    for (int idx : tails_indices) {
        tailsValues.push_back(nums[idx]);
    }

    if (!tailsValues.empty()) {
        drawRow("TAILS", tailsValues, tailsValues.size(), idx_tail_update, -1, isUpdateEvent);
    }
    else {
        // ИСПРАВЛЕНИЕ: Выводим полную пустую рамку, чтобы очистить строку,
        // используя фиктивный вектор и максимальный размер, чтобы обеспечить затирание старых данных.
        std::vector<int> emptyVec(n, 0); // Фиктивный вектор размера N
        // Заполняем его "---" для имитации пустого вида
        for (int& val : emptyVec) val = 0;

        // Рисуем пустую рамку, чтобы затереть старый текст, но без фактических данных
        const int CELL_WIDTH = 4;
        int frameColor = GRAY;

        // --- ВЕРХНЯЯ РАМКА ---
        setColor(frameColor);
        std::cout << "        " << u8"┌";
        for (int k = 0; k < n; ++k) {
            for (int w = 0; w < CELL_WIDTH - 1; ++w) std::cout << u8"─";
            if (k < n - 1) std::cout << u8"┬";
        }
        std::cout << u8"┐\n";

        // --- ДАННЫЕ (пусто) ---
        setColor(DEFAULT);
        std::cout << std::setw(7) << std::left << "TAILS: ";
        setColor(frameColor);
        std::cout << u8"│";

        setColor(GRAY); // Используем серый, чтобы показать, что это заглушка/пусто
        std::string emptyCell = "";
        for (int w = 0; w < CELL_WIDTH - 1; ++w) emptyCell += " ";

        // Заполняем строкой заглушки, чтобы гарантировать очистку
        for (int k = 0; k < n; ++k) {
            std::cout << emptyCell;
            setColor(frameColor);
            std::cout << u8"│";
        }
        std::cout << "\n";

        // --- НИЖНЯЯ РАМКА ---
        setColor(frameColor);
        std::cout << "        " << u8"└";
        for (int k = 0; k < n; ++k) {
            for (int w = 0; w < CELL_WIDTH - 1; ++w) std::cout << u8"─";
            if (k < n - 1) std::cout << u8"┴";
        }
        std::cout << u8"┘\n";
    }

    std::cout << "\n";
    setColor(GREEN);  std::cout << " i "; setColor(DEFAULT); std::cout << "= current item";
    setColor(YELLOW); std::cout << "   Update!";
    setColor(DEFAULT); std::cout << "       \n";
}

// ==========================================
// АЛГОРИТМ O(N^2) (Визуализация)
// ==========================================

inline std::vector<int> getLIS_n2_v(const std::vector<int>& nums)
{
    hideCursor();
    if (nums.empty()) return {};

    const int n = nums.size();
    std::vector<int> dp(n, 1);
    std::vector<int> parent(n, -1);

    int maxLength = 1;
    int maxIndex = 0;

    printState(nums, dp, parent, 0, -1, "Start.");
    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (int i = 0; i < n; ++i)
    {
        printState(nums, dp, parent, i, -1, "Calculating dp[" + std::to_string(i) + "]...");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        for (int j = 0; j < i; ++j)
        {
            std::string msg = "Compare: " + std::to_string(nums[j]) + " < " + std::to_string(nums[i]) + " ?";
            printState(nums, dp, parent, i, j, msg);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            if (nums[j] < nums[i])
            {
                if (dp[j] + 1 > dp[i])
                {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                    printState(nums, dp, parent, i, j,
                        "UPDATE! dp[" + std::to_string(i) + "] becomes " + std::to_string(dp[i]),
                        true);
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                }
            }
        }
        if (dp[i] > maxLength) { maxLength = dp[i]; maxIndex = i; }
    }

    std::vector<int> lis;
    int curr = maxIndex;
    while (curr != -1) { lis.push_back(nums[curr]); curr = parent[curr]; }
    std::reverse(lis.begin(), lis.end());

    resetCursor(); setColor(DEFAULT);
    return lis;
}

// ==========================================
// АЛГОРИТМ O(N Log N) (Визуализация)
// ==========================================

inline std::vector<int> getLIS_nlogn_v(const std::vector<int>& nums)
{
    hideCursor();
    if (nums.empty()) return {};
    int n = nums.size();

    std::vector<int> tails_indices;
    std::vector<int> parent(n, -1);

    printStateNLogN(nums, parent, tails_indices, -1, -1, "Start Patience Sorting...");
    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (int i = 0; i < n; ++i)
    {
        int currentVal = nums[i];

        printStateNLogN(nums, parent, tails_indices, i, -1, "Processing nums[" + std::to_string(i) + "] = " + std::to_string(currentVal));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Сценарий 1: Расширение (Extend)
        if (tails_indices.empty() || currentVal > nums[tails_indices.back()])
        {
            int tail_idx_in_tails = tails_indices.empty() ? -1 : (int)tails_indices.size() - 1;

            std::string msg = tails_indices.empty() ? "Empty Tails. Append!" :
                "Extend? " + std::to_string(currentVal) + " > " + std::to_string(nums[tails_indices.back()]) + " (YES)";

            printStateNLogN(nums, parent, tails_indices, i, tail_idx_in_tails, msg);
            std::this_thread::sleep_for(std::chrono::milliseconds(800));

            if (!tails_indices.empty()) {
                parent[i] = tails_indices.back();
            }

            tails_indices.push_back(i);

            printStateNLogN(nums, parent, tails_indices, i, tails_indices.size() - 1,
                "UPDATE! Tails extended to length " + std::to_string(tails_indices.size()), true);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
        // Сценарий 2: Замена (Replace/Binary Search)
        else
        {
            printStateNLogN(nums, parent, tails_indices, i, tails_indices.size() - 1,
                "Extend? " + std::to_string(currentVal) + " > " + std::to_string(nums[tails_indices.back()]) + " (NO)");
            std::this_thread::sleep_for(std::chrono::milliseconds(800));

            printStateNLogN(nums, parent, tails_indices, i, -1, "Searching for replacement spot...");
            std::this_thread::sleep_for(std::chrono::milliseconds(600));

            // Binary Search
            int l = 0, r = tails_indices.size();
            while (l < r) {
                int mid = l + (r - l) / 2;
                if (nums[tails_indices[mid]] >= currentVal) r = mid;
                else l = mid + 1;
            }

            int foundIndexInTails = l;
            int oldValue = nums[tails_indices[foundIndexInTails]];

            printStateNLogN(nums, parent, tails_indices, i, foundIndexInTails,
                "Found target: replace " + std::to_string(oldValue) + " at Tails[" + std::to_string(foundIndexInTails) + "]");
            std::this_thread::sleep_for(std::chrono::milliseconds(800));

            // Заменяем
            tails_indices[foundIndexInTails] = i;
            if (foundIndexInTails > 0) parent[i] = tails_indices[foundIndexInTails - 1];

            printStateNLogN(nums, parent, tails_indices, i, foundIndexInTails,
                "UPDATE! " + std::to_string(oldValue) + " replaced by " + std::to_string(currentVal), true);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
    }

    printStateNLogN(nums, parent, tails_indices, n - 1, -1, "Finished! Reconstructing path...");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::vector<int> lis;
    if (tails_indices.empty()) return {};
    int curr = tails_indices.back();
    while (curr != -1) {
        lis.push_back(nums[curr]);
        curr = parent[curr];
    }
    reverse(lis.begin(), lis.end());

    resetCursor(); setColor(DEFAULT);
    return lis;
}