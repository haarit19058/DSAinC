#include <bits/stdc++.h>
using namespace std;


// Thin

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    long long ans = 0;
    for (long long i = 5; i <= n; i *= 5) {
        ans += n / i;
    }

    cout << ans << '\n';
    return 0;
}

