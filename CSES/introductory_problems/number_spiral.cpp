#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        long long x, y;
        cin >> x >> y;
        long long z = max(x, y);
        long long ans;

        if (z % 2 == 0) {
            if (x == z)
                ans = z * z - y + 1;
            else
                ans = (z - 1) * (z - 1) + x;
        } else {
            if (y == z)
                ans = z * z - x + 1;
            else
                ans = (z - 1) * (z - 1) + y;
        }

        cout << ans << '\n';
    }

    return 0;
}

