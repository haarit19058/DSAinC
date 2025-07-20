// dice_sum_ways.cpp
// Count number of ways to get sum n by throwing a dice (1-6) one or more times.
// Print result modulo 1e9+7.

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

// Approach 1: Naive recursion (exponential time)
long countWaysNaive(int n) {
    if (n == 0) return 1;
    if (n < 0) return 0;
    long ways = 0;
    for (int dice = 1; dice <= 6; ++dice) {
        ways = (ways + countWaysNaive(n - dice)) % MOD;
    }
    return ways;
}

// Approach 2: Top-down DP with memoization (O(n*6) time, O(n) space)
long countWaysMemo(int n, vector<long>& dp) {
    if (n == 0) return 1;
    if (n < 0) return 0;
    if (dp[n] != -1) return dp[n];
    long ways = 0;
    for (int dice = 1; dice <= 6; ++dice) {
        ways = (ways + countWaysMemo(n - dice, dp)) % MOD;
    }
    return dp[n] = ways;
}

// Approach 3: Bottom-up DP tabulation (O(n*6) time, O(n) space)
long countWaysTab(int n) {
    vector<long> dp(n + 1);
    dp[0] = 1;
    for (int i = 1; i <= n; ++i) {
        long ways = 0;
        for (int dice = 1; dice <= 6; ++dice) {
            if (i - dice >= 0)
                ways = (ways + dp[i - dice]) % MOD;
        }
        dp[i] = ways;
    }
    return dp[n];
}

// Approach 4: Optimized DP using sliding window / prefix sums (O(n) time, O(n) or O(1) extra space)
long countWaysOptimized(int n) {
    // We maintain a deque or circular array of last 6 dp values and their rolling sum
    if (n == 0) return 1;
    deque<long> last6;
    last6.push_back(1); // dp[0]
    long sumLast6 = 1;
    long dp_i = 0;
    for (int i = 1; i <= n; ++i) {
        dp_i = sumLast6;
        if (last6.size() == 6) {
            sumLast6 = (sumLast6 - last6.front() + MOD) % MOD;
            last6.pop_front();
        }
        last6.push_back(dp_i);
        sumLast6 = (sumLast6 + dp_i) % MOD;
    }
    return dp_i;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    
    // Choose method below:
    // 1) Naive (only for small n):
    // cout << countWaysNaive(n) << '\n';

    // 2) Memoization:
    // vector<long> dp(n + 1, -1);
    // cout << countWaysMemo(n, dp) << '\n';

    // 3) Tabulation:
    // cout << countWaysTab(n) << '\n';

    // 4) Optimized sliding-window DP (recommended):
    cout << countWaysOptimized(n) << '\n';

    return 0;
}
