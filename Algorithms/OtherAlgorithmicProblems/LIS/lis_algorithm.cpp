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

std::vector<int> getLIS_n2(const std::vector<int>& nums)
{
    int n = nums.size();
    std::vector<int> parent(n, -1);
    std::vector<int> dp(n, 1);
    int maxLength = 1;
    int maxIndex = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            // если находим элемент меньше текущего и для него LIS длиннее
            if (nums[j] < nums[i] && dp[j] + 1 > dp[i])
            {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
        // обновляем индекс и длину если нашли более длинную LIS
        if (dp[i] > maxLength)
        {
            maxLength = dp[i];
            maxIndex = i;
        }
    }

    std::vector<int> lis;
    for (int i = 0; i < maxLength; i++)
    {
        lis.push_back(nums[maxIndex]);
        maxIndex = parent[maxIndex];
    }
    std::reverse(lis.begin(), lis.end());

    return lis;
}
