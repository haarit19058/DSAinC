#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7; // or any prime like 998244353

int power(int a, int b) {
    int result = 1;
    a %= MOD;  // In case a >= MOD

    while (b > 0) {
        if (b & 1) {
            result = (1LL * result * a) % MOD; // Use 1LL to avoid overflow
        }
        a = (1LL * a * a) % MOD;
        b >>= 1;
    }
    return result;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // TODO: solve "Bit Strings"
	int n;
	cin>>n;
	
	cout<<power(2,n)<<endl;



    return 0;
}
