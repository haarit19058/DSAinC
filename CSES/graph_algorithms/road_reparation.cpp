#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, r;
    DSU(int n): p(n+1), r(n+1,0) {
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
        return p[x]==x ? x : p[x]=find(p[x]);
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a==b) return false;
        if (r[a]<r[b]) swap(a,b);
        p[b] = a;
        if (r[a]==r[b]) r[a]++;
        return true;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector< tuple<long long,int,int> > edges;
    edges.reserve(m);
    for(int i = 0; i < m; i++){
        int a, b;
        long long c;
        cin >> a >> b >> c;
        edges.emplace_back(c, a, b);
    }

    // Sort edges by cost ascending
    sort(edges.begin(), edges.end(),
         [](auto &e1, auto &e2){
             return get<0>(e1) < get<0>(e2);
         });

    DSU dsu(n);
    long long total = 0;
    int used = 0;
    for(auto &e : edges){
        long long c;
        int u, v;
        tie(c, u, v) = e;
        if(dsu.unite(u, v)){
            total += c;
            if(++used == n-1) break;
        }
    }

    if(used == n-1){
        cout << total << "\n";
    } else {
        cout << "IMPOSSIBLE\n";
    }

    return 0;
}

