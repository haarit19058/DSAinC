#include <bits/stdc++.h>
using namespace std;

class DSU {
private:
    vector<int> parent, size;

public:
    int components;
    int maxSize;

    DSU(int n) {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        components = n;
        maxSize = 1;

        for (int i = 1; i <= n; ++i)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]); // Path compression
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return;

        // Union by size
        if (size[ra] < size[rb]) swap(ra, rb);
        parent[rb] = ra;
        size[ra] += size[rb];
        maxSize = max(maxSize, size[ra]);
        components--;
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    DSU dsu(n);

    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        dsu.unite(a, b);
        cout << dsu.components << " " << dsu.maxSize << "\n";
    }

    return 0;
}
