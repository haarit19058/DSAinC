#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    struct Edge { int u, v; long long w; };
    vector<Edge> edges(m);

    for(int i = 0; i < m; i++){
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    const long long INF = LLONG_MAX / 4;
    vector<long long> dist(n+1, 0);
    vector<int> parent(n+1, -1);

    int updated = -1;
    // Relax edges n times
    for(int i = 1; i <= n; i++){
        updated = -1;
        for(auto &e : edges){
            if(dist[e.u] + e.w < dist[e.v]){
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                updated = e.v;
            }
        }
    }

    if(updated == -1){
        // No negative cycle found
        cout << "NO\n";
    } else {
        // There is a negative cycle; find one
        int x = updated;
        // Move x into the cycle
        for(int i = 0; i < n; i++){
            x = parent[x];
        }
        // Extract the cycle
        vector<int> cycle;
        int cur = x;
        do {
            cycle.push_back(cur);
            cur = parent[cur];
        } while(cur != x);
        cycle.push_back(x);
        reverse(cycle.begin(), cycle.end());

        cout << "YES\n";
        for(int node : cycle){
            cout << node << ' ';
        }
        cout << "\n";
    }

    return 0;
}

