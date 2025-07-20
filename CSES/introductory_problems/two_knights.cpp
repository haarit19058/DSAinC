#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n;
    cin >> n;

    for (long long k = 1; k <= n; ++k) {
        long long total_pairs = (k * k) * (k * k - 1) / 2;
        long long attack_pairs = 0;
        if (k > 2) {
            attack_pairs = 4 * (k - 1) * (k - 2);
        }
        cout << total_pairs - attack_pairs << '\n';
    }

    return 0;
}

