class Solution {
public:
    int superEggDrop(int k, int n) {
        vector<int> dp(k + 1, 0);
        int m = 0;
        while (dp[k] < n) {
            m++;
            for (int i = k; i >= 1; --i) {
                dp[i] = dp[i] + dp[i - 1] + 1;
            }
        }
        return m;
    }
};
