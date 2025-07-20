

// thinking aloud 
//
// First appraoch is always brute force
// for all sizes till n/2 traverse along the array and check for each sizes
//
// this will be done in order n**3
//
//
//
// Now we want to optimize the solition maybe in O(n) or O(nlogn)
//
//
// for all the character if we store their positions
//
// Knuth Morris Pratt prefix function which allows computing in O(n) time
//
// 1. compute the prefix function using the kmp algorithm.
// pi[i] length of longest proper prefix which is also a suffix for s[0..i]
//
// 2. From the final value pi[n-1] you can walk backwards through the prefix links to find all valid paths.
//
//


#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    int n = s.size();

    // Compute prefix‐function π[i] = length of longest proper prefix of s[0..i]
    // that is also a suffix of s[0..i].
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            j++;
        }
        pi[i] = j;
    }

    // Collect all periods of the entire string:
    // A length p is a period if for all i >= p, s[i] == s[i - p].
    // Equivalently, (n - p) must be a “border” length of s (i.e. a proper prefix == suffix).
    vector<int> periods;
    int b = pi[n - 1];
    while (b > 0) {
        periods.push_back(n - b);
        b = pi[b - 1];
    }
    // Finally, the full length n is always a trivial period (the string “repeats” once).
    periods.push_back(n);

    // The way we collected periods guarantees they are in strictly increasing order.
    // (Because each border b is strictly decreasing, so n−b is strictly increasing.)
    for (int i = 0; i < (int)periods.size(); i++) {
        if (i) cout << ' ';
        cout << periods[i];
    }
    cout << "\n";

    return 0;
}

