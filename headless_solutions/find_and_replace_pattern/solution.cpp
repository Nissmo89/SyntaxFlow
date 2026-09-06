class Solution {
public:
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string> res;
        vector<int> p = normalize(pattern);
        for (const string& w : words) {
            if (normalize(w) == p) res.push_back(w);
        }
        return res;
    }

private:
    vector<int> normalize(const string& s) {
        unordered_map<char, int> m;
        vector<int> res;
        for (char c : s) {
            if (m.find(c) == m.end()) m[c] = m.size();
            res.push_back(m[c]);
        }
        return res;
    }
};
