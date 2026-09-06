// C++ implementation matching signature
class Solution {
public:
    string crackSafe(int n, int k) {
        if (k == 1) return string(n, '0');          // only "0"*n
        if (n == 1) {                               // any order of 0..k-1
            string res;
            for (int i = 0; i < k; ++i) res.push_back('0' + i);
            return res;
        }

        unordered_set<string> visited;
        string ans;
        string start(n - 1, '0');

        function<void(const string&)> dfs = [&](const string& node) {
            for (int d = 0; d < k; ++d) {
                string next = node.substr(1) + char('0' + d);
                if (!visited.count(next)) {
                    visited.insert(next);
                    dfs(next);
                    ans.push_back(char('0' + d));
                }
            }
        };

        dfs(start);
        reverse(ans.begin(), ans.end());
        return string(n, '0') + ans;
    }
};
