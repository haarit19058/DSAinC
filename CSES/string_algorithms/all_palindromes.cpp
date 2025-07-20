#include <bits/stdc++.h>
using namespace std;

struct Eertree {
    struct Node {
        int next[26];
        int len;
        int link;
        Node(int l = 0) : len(l), link(0) {
            fill(begin(next), end(next), 0);
        }
    };

    vector<Node> tree;
    string s;
    int suff; // node of the longest suffix-palindrome

    Eertree(int n) {
        tree.reserve(n + 3);
        // create roots
        tree.emplace_back(-1); // 0: imaginary root with len = -1
        tree[0].link = 0;
        tree.emplace_back(0);  // 1: empty string root
        tree[1].link = 0;
        suff = 1;
        s.reserve(n);
    }

    int add_char(char ch, int pos) {
        int c = ch - 'a';
        s.push_back(ch);
        int cur = suff;
        // find longest palindrome p such that s[pos - 1 - tree[cur].len] == ch
        while (true) {
            int curlen = tree[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == ch)
                break;
            cur = tree[cur].link;
        }
        if (tree[cur].next[c]) {
            // already exists
            suff = tree[cur].next[c];
            return tree[suff].len;
        }
        // create new node
        Node newnode(tree[cur].len + 2);
        tree.push_back(newnode);
        int newNodeIdx = tree.size() - 1;
        tree[cur].next[c] = newNodeIdx;

        if (tree[newNodeIdx].len == 1) {
            tree[newNodeIdx].link = 1;
        } else {
            // find link for new node
            int linkCandidate = tree[cur].link;
            while (true) {
                int candlen = tree[linkCandidate].len;
                if (pos - 1 - candlen >= 0 && s[pos - 1 - candlen] == ch)
                    break;
                linkCandidate = tree[linkCandidate].link;
            }
            tree[newNodeIdx].link = tree[linkCandidate].next[c];
        }
        suff = newNodeIdx;
        return tree[suff].len;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string str;
    cin >> str;
    int n = str.size();

    Eertree ert(n);
    vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = ert.add_char(str[i], i);
    }

    for (int i = 0; i < n; ++i) {
        cout << ans[i] << (i + 1 < n ? ' ' : '\n');
    }
    return 0;
}

