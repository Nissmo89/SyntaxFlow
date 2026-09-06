class Solution {
public:
    int maxSumAfterPartitioning(vector<int>& arr, int k) {
        int n = arr.size();
        vector<int> dp(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            int curMax = 0;
            for (int j = 1; j <= k && i - j >= 0; ++j) {
                curMax = max(curMax, arr[i - j]);
                dp[i] = max(dp[i], dp[i - j] + curMax * j);
            }
        }
        return dp[n];
    }
};
