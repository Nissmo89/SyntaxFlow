class Solution {
public:
    vector<int> movesToStamp(string stamp, string target) {
        int n = target.size(), m = stamp.size();
        vector<int> res;
        vector<bool> visited(n, false);
        int stars = 0;
        
        while (stars < n) {
            bool replaced = false;
            for (int i = 0; i <= n - m; ++i) {
                if (!visited[i] && canReplace(target, i, stamp)) {
                    stars += doReplace(target, i, m);
                    visited[i] = true;
                    res.push_back(i);
                    replaced = true;
                }
            }
            if (!replaced) return {};
        }
        reverse(res.begin(), res.end());
        return res;
    }

private:
    bool canReplace(const string& t, int pos, const string& s) {
        for (int i = 0; i < s.size(); ++i) {
            if (t[pos + i] != '?' && t[pos + i] != s[i]) return false;
        }
        return true;
    }
    int doReplace(string& t, int pos, int m) {
        int count = 0;
        for (int i = 0; i < m; ++i) {
            if (t[pos + i] != '?') {
                t[pos + i] = '?';
                count++;
            }
        }
        return count;
    }
};
