#include "lis_algorithm.h"

#include <vector>
#include <algorithm>

// O(N^2) - классическое динамическое программирование
std::vector<int> getLIS_n2(const std::vector<int>& nums)
{
    if (nums.empty()) return {};

    const int n = nums.size();

    // dp[i] = длина LIS заканчивающейся на индексе i
    std::vector<int> dp(n, 1);
    // parent[i] = индекс предыдущего элемента для элемента i
    std::vector<int> parent(n, -1);

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
        // обновляем если нашли более длинную LIS
        if (dp[i] > maxLength)
        {
            maxLength = dp[i];
            maxIndex = i;
        }
    }

    // восстановление пути
    std::vector<int> lis;
    int curr = maxIndex;
    while (curr != -1)
    {
        lis.push_back(nums[curr]);
        curr = parent[curr];
    }
    // разворачиваем, т.к. собирали с конца
    reverse(lis.begin(), lis.end());
    return lis;
}

int binSearch(const std::vector<int>& tails, const std::vector<int>& nums, int x)
{
    int l = 0, r = tails.size();
    while (l < r)
    {
        int mid = l + (r - l) / 2;
        if (nums[tails[mid]] >= x)
        {
            r = mid;
        }
        else
        {
            l = mid + 1;
        }
    }
    return l;
}

std::vector<int> getLIS_nlogn(const std::vector<int>& nums)
{
    if (nums.empty()) return {};

    int n = nums.size();

    // tails[i] хранит индекс элемента в nums, который заканчивает LIS длины i + 1
    std::vector<int> tails;

    // parent[i] хранит индекс элемента, который стоит перед nums[i] в LIS
    std::vector<int> parent(n, -1);

    for (int i = 0; i < n; ++i)
    {
        // если продолжение самой длинной цепочки
        if (tails.empty() || nums[i] > nums[tails.back()])
        {
            if (!tails.empty())
            {
                parent[i] = tails.back();
            }
            tails.push_back(i);
        }
        // ищем место для замены, т.к. нашли число меньше конечного
        else
        {
            int idx = binSearch(tails, nums, nums[i]);
            tails[idx] = i;

            // если вставили не в начало
            if (idx > 0)
            {
                parent[i] = tails[idx - 1];
            }
        }
    }
    // восстановление пути
    std::vector<int> lis;

    if (tails.empty()) return {};

    int curr = tails.back();
    while (curr != -1)
    {
        lis.push_back(nums[curr]);
        curr = parent[curr];
    }
    reverse(lis.begin(), lis.end());

    return lis;
}
