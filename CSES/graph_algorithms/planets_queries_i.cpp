#include <bits/stdc++.h>
using namespace std;


/*
A clean way to answer “where do you end up after k teleports from x?” in O(logk) per query (after an O(nlogK) preprocess, where K is the max 𝑘k) is to build a binary‑lifting table of size ⌈log2𝐾⌉. Here’s a C++ implementation (with 𝐾K up to 10910 9 , so log⁡2𝐾≈30log 2​ K≈30):
*/


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> t(n+1);
    for(int i = 1; i <= n; i++){
        cin >> t[i];
    }

    // Precompute up[j][i] = the planet you reach from i after 2^j teleports.
    const int LOG = 30;           // since k ≤ 1e9 < 2^30
    vector<array<int,LOG>> up(n+1);
    // 2^0 = 1 step:
    for(int i = 1; i <= n; i++){
        up[i][0] = t[i];
    }
    // Build table for 2^j steps:
    for(int j = 1; j < LOG; j++){
        for(int i = 1; i <= n; i++){
            up[i][j] = up[ up[i][j-1] ][j-1];
        }
    }

    // Answer queries:
    while(q--){
        int x;
        long long k;
        cin >> x >> k;
        for(int j = 0; j < LOG; j++){
            if(k & (1LL << j)){
                x = up[x][j];
            }
        }
        cout << x << "\n";
    }

    return 0;
}

