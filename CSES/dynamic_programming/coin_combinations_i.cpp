#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

// dp[target] will store the number of combinations to make sum `target`
int solve(vector<int> &coins, int target, vector<int>& dp) {
    if (target == 0) return 1;
    if (target < 0) return 0;

    if (dp[target] != -1) return dp[target];

    int count = 0;
    for (int coin : coins) {
        count = (count + solve(coins, target - coin, dp)) % MOD;
    }

    return dp[target] = count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }

    vector<int> dp(x + 1, -1);
    int result = solve(coins, x, dp);

    cout << result << endl;

    return 0;
}

