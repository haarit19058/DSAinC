#include <bits/stdc++.h>

using namespace std;

#define fast_io cin.tie(nullptr);ios::sync_with_stdio(false);

int T, a, b;

int main(){
	fast_io
    //scanf("%d", &T);
	cin>>T;
    for(int t = 0; t < T; t++){
        cin>>a>>b;
        printf(((a+b)%3 != 0 || a > 2*b || b > 2*a) ? "NO\n" : "YES\n");
    }
}
