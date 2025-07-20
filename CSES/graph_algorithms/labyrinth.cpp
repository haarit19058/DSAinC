#include <bits/stdc++.h>
using namespace std;

#define fast_io ios::sync_with_stdio(false); cin.tie(nullptr);

int main() {
    fast_io;
    int n, m;
    cin >> n >> m;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    pair<int,int> start, finish;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'A') start = {i,j};
            else if (grid[i][j] == 'B') finish = {i,j};
        }
    }

    // 4 directions: U,D,L,R
    const vector<pair<int,int>> dirs = {
        {-1,0}, {+1,0}, {0,-1}, {0,+1}
    };
    const vector<char>   dch  = {'U','D','L','R'};

    // parent pointers and visited flag
    vector<vector<bool>> vis(n, vector<bool>(m,false));
    vector<vector<pair<int,int>>> parent(n, vector<pair<int,int>>(m, {-1,-1}));
    vector<vector<char>> moveTaken(n, vector<char>(m, '?'));

    queue<pair<int,int>> q;
    q.push(start);
    vis[start.first][start.second] = true;

    bool found = false;
    while (!q.empty() && !found) {
        auto [x,y] = q.front(); q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = x + dirs[i].first;
            int ny = y + dirs[i].second;
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            if (vis[nx][ny] || grid[nx][ny] == '#') continue;
            vis[nx][ny] = true;
            parent[nx][ny] = {x,y};
            moveTaken[nx][ny] = dch[i];
            if (nx == finish.first && ny == finish.second) {
                found = true;
                break;
            }
            q.push({nx,ny});
        }
    }

    if (!vis[finish.first][finish.second]) {
        cout << "NO\n";
        return 0;
    }

    // Reconstruct path
    string path;
    auto cur = finish;
    while (cur != start) {
        char c = moveTaken[cur.first][cur.second];
        path.push_back(c);
        cur = parent[cur.first][cur.second];
    }
    reverse(path.begin(), path.end());

    cout << "YES\n";
    cout << path.size() << "\n";
    cout << path << "\n";
    return 0;
}

