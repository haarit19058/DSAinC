#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;
    vector<int> price(n), pages(n);
    for (int i = 0; i < n; i++)
        cin >> price[i];
    for (int i = 0; i < n; i++)
        cin >> pages[i];

    // dp[c] = max pages we can get with total cost exactly c
    // we only need costs up to x
    vector<long long> dp(x+1, 0);

    // for each book, sweep costs backwards so we don’t reuse the same book twice
    for (int i = 0; i < n; i++) {
        int h = price[i];
        int s = pages[i];
        for (int c = x; c >= h; c--) {
            dp[c] = max(dp[c], dp[c - h] + s);
        }
    }

    // answer is the best pages for any cost ≤ x
    long long ans = *max_element(dp.begin(), dp.end());
    cout << ans << "\n";
    return 0;
}







// #include <iostream>
// #include <vector>
// #include <cstring> // for memset
// using namespace std;

// const int MAXN = 1005;
// const int MAXX = 1e5 + 5;

// int n, x;
// int h[MAXN]; // prices
// int s[MAXN]; // pages
// int dp[MAXN][MAXX]; // memo table

// // Recursive function with memoization
// int solve(int i, int remaining) {
//     // Base case: no books left or no budget left
//     if (i == n || remaining == 0) return 0;

//     // If already computed
//     if (dp[i][remaining] != -1) return dp[i][remaining];

//     // Option 1: skip the current book
//     int ans = solve(i + 1, remaining);

//     // Option 2: take the book if we can afford it
//     if (h[i] <= remaining) {
//         ans = max(ans, solve(i + 1, remaining - h[i]) + s[i]);
//     }

//     return dp[i][remaining] = ans;
// }

// int main() {
//     cin >> n >> x;

//     for (int i = 0; i < n; ++i) cin >> h[i];
//     for (int i = 0; i < n; ++i) cin >> s[i];

//     memset(dp, -1, sizeof(dp)); // initialize all states as uncomputed

//     cout << solve(0, x) << endl;

//     return 0;
// }
