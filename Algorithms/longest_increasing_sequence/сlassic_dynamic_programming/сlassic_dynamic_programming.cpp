#include <iostream>
#include <vector>

using namespace std;

template <typename T>
void print_vec(const vector<T>& v)
{
    for (const auto& x : v) cout << x << " ";
    cout << "\n";
}

template <typename T>
vector<T> lis_n2(const vector<T>& a)
{
    size_t n = a.size();
    if (n == 0) return {};

    vector<int> dp(n, 1);
    vector<int> prev(n, -1);

    int max_len = 1;
    int max_end = 0;

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            if (a[j] < a[i] && dp[j] + 1 > dp[i])
            {
                dp[i] = dp[j] + 1;
                prev[i] = static_cast<int>(j);
            }
        }
        if (dp[i] > max_len)
        {
            max_len = dp[i];
            max_end = static_cast<int>(i);
        }
    }

    // итеративное восстановление
    vector<T> seq;
    for (int cur = max_end; cur != -1; cur = prev[cur])
    {
        seq.push_back(a[cur]);
    }
    reverse(seq.begin(), seq.end());
    return seq;
}

int main()
{
    setlocale(LC_ALL, "rus");

    vector<int> a = { 5, 10, 6, 12, 3, 24, 7, 8 };
    
    cout << "input: ";
    print_vec(a);

    auto seq = lis_n2(a);
    cout << "\nlis O(n^2): " << seq.size() << "\n-> ";
    print_vec(seq);

    return 0;
}
