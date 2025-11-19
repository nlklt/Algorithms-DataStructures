#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
void print_vec(const vector<T>& a)
{
    for (const auto& x : a) cout << x << " ";
    cout << "\n";
}

template <typename T>
int bin_search(const vector<T>& tails, const T& x)
{
    int idx = 0, l = 0, r = tails.size() - 1;
    while (l < r)
    {
        idx = l + (r - l) / 2;
        if (tails[idx] >= x) r = idx;
        else l = idx + 1;
    }
    return l;
}

template <typename T>
vector<T> lis_nlogn(const vector<T>& a)
{
    if (a.empty()) return {};
    
    vector<T> tails;

    for (const auto& x : a)
    {
        if (tails.empty() || x > tails.back())
        {
            tails.push_back(x);
        }
        else
        {
            int idx = bin_search(tails, x);
            tails[idx] = x;
        }
    }
    return tails;
}

int main()
{
    setlocale(LC_ALL, "rus");

    vector<int> a = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };

    cout << "input: ";
    print_vec(a);

    vector<int> seq = lis_nlogn(a);
    cout << "lis nlogn: " << seq.size() << "\n-> ";
    print_vec(seq);
}
