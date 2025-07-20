#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<pair<int, int>> arr(n);
    for (int i = 0; i < n; ++i) {
        cin >> arr[i].first;
        arr[i].second = i + 1; // 1-based indexing
    }

    unordered_map<int, pair<int, int>> mp;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int current_sum = arr[i].first + arr[j].first;
            int required = x - current_sum;
            if (mp.count(required)) {
                auto [a, b] = mp[required];
                // ensure all indices are distinct
                if (a != i && a != j && b != i && b != j) {
                    cout << a + 1 << " " << b + 1 << " " << i + 1 << " " << j + 1 << "\n";
                    return 0;
                }
            }
        }
        for (int k = 0; k < i; ++k) {
            mp[arr[i].first + arr[k].first] = {k, i};
        }
    }

    cout << "IMPOSSIBLE\n";
    return 0;
}
