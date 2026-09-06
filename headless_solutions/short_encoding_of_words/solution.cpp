class Solution {
public:
    int minimumLengthEncoding(vector<string>& words) {
        unordered_set<string> s(words.begin(), words.end());
        for (const string& w : words) {
            for (int i = 1; i < w.size(); ++i) {
                s.erase(w.substr(i));
            }
        }
        int res = 0;
        for (const string& w : s) {
            res += w.size() + 1;
        }
        return res;
    }
};
