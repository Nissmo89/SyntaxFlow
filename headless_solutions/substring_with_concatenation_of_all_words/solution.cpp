class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        if (s.empty() || words.empty()) return res;
        int n = s.size(), m = words.size(), len = words[0].size();
        unordered_map<string, int> counts;
        for (const string& w : words) counts[w]++;
        
        for (int i = 0; i < len; ++i) {
            unordered_map<string, int> seen;
            int left = i, count = 0;
            for (int j = i; j <= n - len; j += len) {
                string w = s.substr(j, len);
                if (counts.count(w)) {
                    seen[w]++;
                    count++;
                    while (seen[w] > counts[w]) {
                        string leftW = s.substr(left, len);
                        seen[leftW]--;
                        count--;
                        left += len;
                    }
                    if (count == m) res.push_back(left);
                } else {
                    seen.clear();
                    count = 0;
                    left = j + len;
                }
            }
        }
        return res;
    }
};
