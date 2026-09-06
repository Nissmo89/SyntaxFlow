#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numSpecialEquivGroups(vector<string>& words) {
        unordered_set<string> uniq;               // stores canonical signatures
        for (const string& w : words) {
            string even, odd;
            even.reserve(w.size() / 2 + 1);
            odd.reserve(w.size() / 2 + 1);
            for (size_t i = 0; i < w.size(); ++i) {
                if (i % 2 == 0) even.push_back(w[i]);
                else            odd.push_back(w[i]);
            }
            sort(even.begin(), even.end());
            sort(odd.begin(), odd.end());
            string sig = even + "#" + odd;         // '#' cannot appear in the strings
            uniq.insert(sig);
        }
        return static_cast<int>(uniq.size());
    }
};
