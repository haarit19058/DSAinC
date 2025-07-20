#include <bits/stdc++.h>
using namespace std;
static const int MOD = 1'000'000'007;

// Fenwick tree for range-sum on [1..N]
struct Fenwick {
    int N;
    vector<int> f;
    Fenwick(int _N): N(_N), f(N+1, 0) {}
    // add v into index i
    void update(int i, int v) {
        for (; i <= N; i += i & -i) {
            f[i] = (f[i] + v) % MOD;
        }
    }
    // sum of [1..i]
    int query(int i) const {
        long long s = 0;
        for (; i > 0; i -= i & -i) {
            s += f[i];
        }
        return s % MOD;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for(int &x: a) cin >> x;

    // 1) Coordinate compression
    vector<int> vals = a;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    auto comp = [&](int x){
        return int(lower_bound(vals.begin(), vals.end(), x) - vals.begin()) + 1;
    };
    int m = vals.size();

    // 2) Fenwick tree over [1..m]
    Fenwick fenw(m);

    long long answer = 0;
    // 3) Process each element
    for(int i = 0; i < n; i++){
        int c = comp(a[i]);                 // compressed value in [1..m]
        int sum_less = fenw.query(c - 1);   // sum of dp[j] with comp(a[j]) < c
        int dpi      = (1 + sum_less) % MOD;
        fenw.update(c, dpi);
        answer = (answer + dpi) % MOD;
    }

    cout << answer << "\n";
    return 0;
}

