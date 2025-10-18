#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e6+10;
typedef long long int64;

int64 func(const int64 n)
{
    if (n <= 1) return 1;
    return func(n - 1) * n;
}

int main()
{
    cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);

    int64 n; cin >> n;
    cout << func(n) << "\n";

    return 0;
}
