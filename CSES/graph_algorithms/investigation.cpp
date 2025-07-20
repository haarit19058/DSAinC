#include <bits/stdc++.h>
using namespace std;

using ll = long long;
static const ll INF = (ll)4e18;
static const int MOD = 1e9+7;

struct Edge{
	int to;
	ll w;
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


	int n, m;
    cin >> n >> m;
    vector<vector<Edge>> adj(n+1);
    for(int i = 0; i < m; i++){
        int a, b;
        ll c;
        cin >> a >> b >> c;
        adj[a].push_back({b,c});
    }

	// what is the min price : dijkstra will do
	//
	// how many min price routes??
	//
	//
	// What is min number of flights in a min price routce 
	//
	// what is max number of flights
	//
	//
	//single modified dijkstra that keeps for every city 
	//dist[v] = minimum cost from 1 to v
	//cnt[v] = number of minimum cost ways to reach v (mod 1e9 + 7)
	//mnf[v] = minimum number of flights among those min-cost ways
	//mxf[v] = max number of flights among those min cost ways
	//

	// Dijkstra data
    vector<ll> dist(n+1, INF);
    vector<int> cnt(n+1, 0), mnf(n+1, INT_MAX), mxf(n+1, 0);
    dist[1] = 0;
    cnt[1]   = 1;
    mnf[1]   = 0;
    mxf[1]   = 0;

	// Min‑heap: (distance, node)
    priority_queue<pair<ll,int>,
                   vector<pair<ll,int>>,
                   greater<pair<ll,int>>> pq;
    pq.push({0,1});

    while(!pq.empty()){
        auto [d,u] = pq.top(); 
        pq.pop();
        if (d != dist[u]) 
            continue;  // stale entry

        for(auto &e : adj[u]){
            int v = e.to;
            ll nd = d + e.w;

            if (nd < dist[v]) {
                // strictly better cost
                dist[v] = nd;
                cnt[v]  = cnt[u];
                mnf[v]  = mnf[u] + 1;
                mxf[v]  = mxf[u] + 1;
                pq.push({nd, v});
            }
            else if (nd == dist[v]) {
                // same minimum cost → aggregate routes
                cnt[v] = (cnt[v] + cnt[u]) % MOD;
                mnf[v] = min(mnf[v], mnf[u] + 1);
                mxf[v] = max(mxf[v], mxf[u] + 1);
                // no need to push, dist[v] didn’t change
            }
        }
    }


	cout 
      << dist[n] << " "
      << cnt[n]  << " "
      << mnf[n]  << " "
      << mxf[n]  << "\n";


    return 0;
}
