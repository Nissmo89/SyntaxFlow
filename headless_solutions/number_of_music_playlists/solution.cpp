class Solution {
public:
    int numMusicPlaylists(int n, int goal, int k) {
        long long mod = 1e9 + 7;
        vector<vector<long long>> dp(goal + 1, vector<long long>(n + 1, 0));
        dp[0][0] = 1;
        for (int i = 1; i <= goal; ++i) {
            for (int j = 1; j <= n; ++j) {
                // Case 1: New song
                dp[i][j] = (dp[i - 1][j - 1] * (n - j + 1)) % mod;
                // Case 2: Old song
                if (j > k) {
                    dp[i][j] = (dp[i][j] + dp[i - 1][j] * (j - k)) % mod;
                }
            }
        }
        return (int)dp[goal][n];
    }
};
