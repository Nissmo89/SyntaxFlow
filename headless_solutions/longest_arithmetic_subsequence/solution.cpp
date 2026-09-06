class Solution {
public:
    int longestArithSeqLength(vector<int>& nums) {
        int n = nums.size();
        if (n <= 2) return n;
        int res = 2;
        // dp[index][diff + 500]
        vector<vector<int>> dp(n, vector<int>(1001, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                int diff = nums[i] - nums[j] + 500;
                dp[i][diff] = dp[j][diff] > 0 ? dp[j][diff] + 1 : 2;
                res = max(res, dp[i][diff]);
            }
        }
        return res;
    }
};
