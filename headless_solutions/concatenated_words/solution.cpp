#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        // Sort by length so that only shorter words are available in the set
        sort(words.begin(), words.end(),
             [](const string& a, const string& b) { return a.size() < b.size(); });

        unordered_set<string> dict;
        vector<string> ans;

        for (const string& w : words) {
            int n = (int)w.size();
            vector<bool> dp(n + 1, false);
            dp[0] = true;                     // empty prefix

            for (int i = 1; i <= n; ++i) {
                for (int j = 0; j < i; ++j) {
                    if (!dp[j]) continue;
                    string sub = w.substr(j, i - j);
                    if (dict.find(sub) != dict.end()) {
                        dp[i] = true;
                        break;
                    }
                }
            }

            if (dp[n]) ans.push_back(w);
            dict.insert(w);                    // make current word available for longer ones
        }
        return ans;
    }
};
