class Solution {
public:
    int maxCoins(vector<int>& nums) {
        int n = nums.size();
        vector<int> A(n + 2, 1);
        for (int i = 0; i < n; ++i) A[i + 1] = nums[i];
        
        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
        
        for (int len = 1; len <= n; ++len) {
            for (int i = 1; i <= n - len + 1; ++i) {
                int j = i + len - 1;
                for (int k = i; k <= j; ++k) {
                    dp[i][j] = max(dp[i][j], dp[i][k - 1] + A[i - 1] * A[k] * A[j + 1] + dp[k + 1][j]);
                }
            }
        }
        return dp[1][n];
    }
};
