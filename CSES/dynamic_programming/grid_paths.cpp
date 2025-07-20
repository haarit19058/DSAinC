// #include <bits/stdc++.h>
// using namespace std;

// static const int MOD = 1e9+7;
// int n;
// vector<string> grid;
// vector<vector<int>> memo;

// // go(i,j) = number of ways from (i,j) to (n-1,n-1)
// int go(int i, int j) {
//     // Out of bounds or trap → 0 ways
//     if (i < 0 || i >= n || j < 0 || j >= n) return 0;
//     if (grid[i][j] == '*') return 0;

//     // Reached end
//     if (i == n-1 && j == n-1) return 1;

//     // If already computed, return it
//     if (memo[i][j] != -1) 
//         return memo[i][j];

//     // Otherwise compute from down and right
//     long long ways = go(i+1, j);
//     ways = (ways + go(i, j+1)) % MOD;
//     return memo[i][j] = (int)ways;
// }

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);

//     cin >> n;
//     grid.resize(n);
//     for (int i = 0; i < n; i++)
//         cin >> grid[i];

//     memo.assign(n, vector<int>(n, -1));
    
//     // If start is blocked, answer is 0
//     if (grid[0][0] == '*') {
//         cout << 0 << "\n";
//     } else {
//         cout << go(0, 0) << "\n";
//     }
//     return 0;
// }








#include <bits/stdc++.h>
using namespace std;
static const int MOD = 1e9+7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    // dp[j] will hold the dp value for current row, in column j
    vector<int> dp(n, 0);

    // Starting cell
    dp[0] = (grid[0][0] == '.' ? 1 : 0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '*') {
                dp[j] = 0;
            } else if (!(i == 0 && j == 0)) {
                int fromAbove = (i > 0 ? dp[j] : 0);
                int fromLeft  = (j > 0 ? dp[j-1] : 0);
                dp[j] = (fromAbove + fromLeft) % MOD;
            }
        }
    }

    cout << dp[n-1] << "\n";
    return 0;
}
