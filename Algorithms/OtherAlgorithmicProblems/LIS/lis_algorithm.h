#pragma once
#include <vector>

/*
 *  - getLIS_n2:      O(n^2) классическое DP, возвращает саму подпоследовательность (не только длину)
 *  - getLIS_nlogn:   O(n*log(n)) алгоритм (паттерн "tails + parent"), возвращает подпоследовательность
*/

std::vector<int> getLIS_n2(const std::vector<int>& nums);
std::vector<int> getLIS_nlogn(const std::vector<int>& nums);
