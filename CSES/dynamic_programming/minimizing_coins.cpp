#include <bits/stdc++.h>
using namespace std;
static const int INF = 1e9;

// ----------------------------------------------------------------------------
// Approach 1: Recursive + Memoization
// Time  : O(n * x) in total (each state [0..x] computed once, each tries n coins)
// Space : O(x) for memo array + O(x) recursion stack in worst case
// ----------------------------------------------------------------------------
int solveRecMemoUtil(const vector<int>& coins, int x, vector<int>& dp) {
    if (x == 0) return 0;
    if (dp[x] != -1) return dp[x];
    int best = INF;
    for (int c : coins) {
        if (c <= x) {
            int res = solveRecMemoUtil(coins, x - c, dp);
            if (res != INF)
                best = min(best, res + 1);
        }
    }
    return dp[x] = best;
}

int solveRecMemo(const vector<int>& coins, int x) {
    vector<int> dp(x + 1, -1);
    dp[0] = 0;
    int ans = solveRecMemoUtil(coins, x, dp);
    return (ans == INF ? -1 : ans);
}

// ----------------------------------------------------------------------------
// Approach 2: Iterative Bottom‑Up DP
// Time  : O(n * x) (for each sum 1..x, scan all n coins)
// Space : O(x) for dp array
// ----------------------------------------------------------------------------
int solveIterDP(const vector<int>& coins, int x) {
    vector<int> dp(x + 1, INF);
    dp[0] = 0;
    for (int sum = 1; sum <= x; ++sum) {
        for (int c : coins) {
            if (c <= sum && dp[sum - c] + 1 < dp[sum]) {
                dp[sum] = dp[sum - c] + 1;
            }
        }
    }
    return (dp[x] == INF ? -1 : dp[x]);
}

// ----------------------------------------------------------------------------
// Approach 3: BFS over “sum so far” states
// Time  : O(n * x) in worst case (each state 0..x visited once, edges ≤ n each)
// Space : O(x) for visited array + queue
// ----------------------------------------------------------------------------
int solveBFS(const vector<int>& coins, int x) {
    vector<bool> visited(x + 1, false);
    queue<pair<int,int>> q;  // {current_sum, coins_used}
    q.push({0, 0});
    visited[0] = true;
    while (!q.empty()) {
        auto [cur, steps] = q.front(); q.pop();
        for (int c : coins) {
            int nxt = cur + c;
            if (nxt == x) return steps + 1;
            if (nxt < x && !visited[nxt]) {
                visited[nxt] = true;
                q.push({nxt, steps + 1});
            }
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;
    vector<int> coins(n);
    for (int i = 0; i < n; ++i) {
        cin >> coins[i];
    }

    // You can switch between implementations below:
    // 1) Recursive + Memoization
    // int answer = solveRecMemo(coins, x);
    //
    // 2) Iterative DP (default)
    int answer = solveIterDP(coins, x);
    //
    // 3) BFS
    // int answer = solveBFS(coins, x);

    cout << answer << "\n";
    return 0;
}
