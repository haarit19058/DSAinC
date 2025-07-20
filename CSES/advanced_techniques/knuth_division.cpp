/*
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5005;
vector<vector<int>> dp(MAXN, vector<int>(MAXN, -1));
vector<int> prefix_sum;

int get_sum(int l, int r) {
    return prefix_sum[r + 1] - prefix_sum[l];  // sum from l to r inclusive
}

int util1(vector<int>& arr, int l, int r) {
    if (l == r) return 0;

    if (dp[l][r] != -1) return dp[l][r];

    int sum = get_sum(l, r);
    int ans = INT_MAX;

    for (int k = l; k < r; ++k) {
        ans = min(ans, util1(arr, l, k) + util1(arr, k + 1, r) + sum);
    }

    return dp[l][r] = ans;
}

void notoptimal() {
    int n;
    cin >> n;

    vector<int> arr(n);
    prefix_sum.resize(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        prefix_sum[i + 1] = prefix_sum[i] + arr[i];
    }

    int ans = util1(arr, 0, n - 1);
    cout << ans << endl;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // TODO: solve "Knuth Division"
	notoptimal();	

    return 0;
}

*/



#include <bits/stdc++.h>
using namespace std;

/*
  CSES – Knuth Division

  Given an array of n numbers, you must split it down into n single-element subarrays.
  Each move consists of choosing some contiguous subarray [l..r] (initially the whole array)
  and splitting it into two nonempty parts: [l..k] and [k+1..r]. The cost of that move is
  the sum of all elements in the chosen subarray [l..r]. Repeat until every subarray is size 1.
  Compute the minimum total cost.

  This is equivalent to the “optimal merging of adjacent stones” problem (K=2): 
    dp[l][r] = (sum of x[l..r]) + min_{l ≤ k < r} ( dp[l][k] + dp[k+1][r] ), 
    with dp[i][i] = 0.
  We can apply Knuth’s optimization, since this cost function satisfies the quadrangle inequality
  and the required monotonicity condition. Concretely, define:
    sum[l][r] = prefix[r] - prefix[l-1]
    dp[l][r] = minimum cost to split [l..r] fully into singletons
    opt[l][r] = index k (l ≤ k < r) where the optimum split for dp[l][r] occurs

  Recurrence with Knuth’s optimization:
    - Base: dp[i][i] = 0, and opt[i][i] = i.
    - For length = 2..n:
        For l = 1..n-length+1:
          r = l + length - 1
          We only have to try k from opt[l][r-1] .. opt[l+1][r], inclusive.
          Let cost_k = dp[l][k] + dp[k+1][r] + sum[l..r].
          Pick the k in that small range minimizing cost_k, set dp[l][r], opt[l][r].

  Time complexity: O(n^2).  Memory: O(n^2).  n ≤ 5000, so this fits within 512 MB.

  Example:
    n = 5, x = [2, 7, 3, 2, 5]
    The optimal total cost is 43.
*/

static const long long INF = LLONG_MAX;

// We’ll store everything in 1-based indexing.
static long long dp[5001][5001];
static int opt[5001][5001];
static long long prefix[5001];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> x(n+1);
    for (int i = 1; i <= n; i++) {
        cin >> x[i];
    }

    // Build prefix sums
    prefix[0] = 0;
    for (int i = 1; i <= n; i++) {
        prefix[i] = prefix[i-1] + x[i];
    }

    // sum(l, r) = prefix[r] - prefix[l-1]
    auto range_sum = [&](int l, int r) {
        return prefix[r] - prefix[l-1];
    };

    // Base cases: dp[i][i] = 0, opt[i][i] = i
    for (int i = 1; i <= n; i++) {
        dp[i][i] = 0;
        opt[i][i] = i;
    }

    // Fill dp by increasing length
    for (int len = 2; len <= n; len++) {
        for (int l = 1; l + len - 1 <= n; l++) {
            int r = l + len - 1;
            dp[l][r] = INF;

            // Allowed range for the optimal k is [opt[l][r-1], opt[l+1][r]]
            int start = opt[l][r-1];
            int end   = opt[l+1][r];
            if (start > end) {
                // In the rare case the interval is “reversed,” clamp it
                start = l;
                end = r - 1;
            }
            // But also ensure we stay within [l, r-1]
            start = max(start, l);
            end   = min(end, r-1);

            long long totalSum = range_sum(l, r);
            for (int k = start; k <= end; k++) {
                long long cost = dp[l][k] + dp[k+1][r] + totalSum;
                if (cost < dp[l][r]) {
                    dp[l][r] = cost;
                    opt[l][r] = k;
                }
            }
        }
    }

    // The answer for splitting the entire array [1..n]
    cout << dp[1][n] << "\n";
    return 0;
}

