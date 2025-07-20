#include <bits/stdc++.h>
using namespace std;

static const int MOD = 1e9+7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

	int n, x;
    cin >> n >> x;

    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }

    // dp[s] = number of ordered ways to make sum = s
    vector<int> dp(x + 1, 0);
    dp[0] = 1;

    // For each sum from 1 to x, add ways by using each coin
    for (int s = 1; s <= x; s++) {
        long long ways = 0;
        for (int c : coins) {
            if (s - c >= 0) {
                ways += dp[s - c];
                if (ways >= MOD) ways -= MOD;
            }
        }
        dp[s] = ways;
    }

    cout << dp[x] << "\n";
    return 0;
}

