#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, rnk;
    DSU(int n) : parent(n+1), rnk(n+1, 0) {
        for (int i = 1; i <= n; i++)
            parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (rnk[a] < rnk[b]) 
            parent[a] = b;
        else if (rnk[b] < rnk[a]) 
            parent[b] = a;
        else {
            parent[b] = a;
            rnk[a]++;
        }
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DSU dsu(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        dsu.unite(u, v);
    }

    // Gather one representative per connected component
    vector<int> reps;
    reps.reserve(n);
    for (int i = 1; i <= n; i++) {
        if (dsu.find(i) == i)  // i is root of its component
            reps.push_back(i);
    }

    // We need exactly (c - 1) roads to connect c components
    int c = reps.size();
    cout << max(0, c - 1) << "\n";
    for (int i = 1; i < c; i++) {
        // link reps[i-1] <-> reps[i]
        cout << reps[i-1] << " " << reps[i] << "\n";
    }

    return 0;
}

