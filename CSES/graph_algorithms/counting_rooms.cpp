#include <iostream>
#include <vector>
#include <set>
using namespace std;

struct DSU {
    vector<int> parent;
    DSU(int n) : parent(n, -1) {}

    int find(int x) {
        if (parent[x] < 0) return x;
        return parent[x] = find(parent[x]);
    }

    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (parent[a] > parent[b]) swap(a, b);
        parent[a] += parent[b];
        parent[b] = a;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<char>> grid(n, vector<char>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> grid[i][j];

    DSU dsu(n * m);

    auto inBounds = [&](int x, int y) {
        return x >= 0 && x < n && y >= 0 && y < m;
    };

    // Directions: up, down, left, right
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#') continue;
            int curr = i * m + j;
            for (int d = 0; d < 4; ++d) {
                int ni = i + dx[d], nj = j + dy[d];
                if (inBounds(ni, nj) && grid[ni][nj] == '.') {
                    int neighbor = ni * m + nj;
                    dsu.unite(curr, neighbor);
                }
            }
        }
    }

    // Count unique roots (distinct components)
    set<int> components;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (grid[i][j] == '.')
                components.insert(dsu.find(i * m + j));

    cout << components.size() << '\n';

    return 0;
}

