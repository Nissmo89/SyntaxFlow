#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

class Solution {
public:
    string longestWord(vector<string>& words) {
        sort(words.begin(), words.end());
        unordered_set<string> built;
        string res = "";
        for (const string& w : words) {
            if (w.size() == 1 || built.count(w.substr(0, w.size() - 1))) {
                if (w.size() > res.size()) res = w;
                built.insert(w);
            }
        }
        return res;
    }
};
