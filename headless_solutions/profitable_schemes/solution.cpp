class Solution {
public:
    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
        int MOD = 1e9 + 7;
        vector<vector<int>> dp(minProfit + 1, vector<int>(n + 1, 0));
        dp[0][0] = 1;
        
        for (size_t k = 0; k < group.size(); ++k) {
            int g = group[k], p = profit[k];
            for (int i = minProfit; i >= 0; --i) {
                for (int j = n - g; j >= 0; --j) {
                    int next_p = min(i + p, minProfit);
                    dp[next_p][j + g] = (dp[next_p][j + g] + dp[i][j]) % MOD;
                }
            }
        }
        
        int total = 0;
        for (int count : dp[minProfit]) total = (total + count) % MOD;
        return total;
    }
};
