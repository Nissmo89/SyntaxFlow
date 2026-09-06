class Solution {
public:
    int strangePrinter(string s) {
        string t = "";
        for (char c : s) if (t.empty() || t.back() != c) t += c;
        int n = t.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for (int i = n - 1; i >= 0; --i) {
            dp[i][i] = 1;
            for (int j = i + 1; j < n; ++j) {
                dp[i][j] = dp[i][j - 1] + 1;
                for (int k = i; k < j; ++k) {
                    if (t[k] == t[j])
                        dp[i][j] = min(dp[i][j], dp[i][k] + (k + 1 <= j - 1 ? dp[k + 1][j - 1] : 0));
                }
            }
        }
        return dp[0][n - 1];
    }
};
