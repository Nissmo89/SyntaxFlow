#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        int n = s.size(), m = p.size();
        vector<int> ans;
        if (m > n) return ans;                     // impossible

        // frequency of characters in p
        int need[26] = {0};
        for (char c : p) ++need[c - 'a'];

        // frequency of the first window in s
        int window[26] = {0};
        for (int i = 0; i < m; ++i) ++window[s[i] - 'a'];

        // helper to compare two frequency arrays
        auto same = [&]() -> bool {
            for (int i = 0; i < 26; ++i)
                if (need[i] != window[i]) return false;
            return true;
        };

        if (same()) ans.push_back(0);

        // slide the window
        for (int i = m; i < n; ++i) {
            // character leaving the window
            --window[s[i - m] - 'a'];
            // character entering the window
            ++window[s[i] - 'a'];
            if (same()) ans.push_back(i - m + 1);
        }
        return ans;
    }
};
