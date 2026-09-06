class Solution {
public:
    int checkRecord(int n) {
        long MOD = 1e9 + 7;
        // dp[absences][trailing_lates]
        vector<vector<long>> dp(2, vector<long>(3, 0));
        dp[0][0] = 1; // Base case: length 0, 0 absences, 0 trailing lates

        for (int i = 1; i <= n; ++i) {
            vector<vector<long>> next(2, vector<long>(3, 0));
            for (int a = 0; a < 2; ++a) {
                for (int l = 0; l < 3; ++l) {
                    // Add 'P'
                    next[a][0] = (next[a][0] + dp[a][l]) % MOD;
                    // Add 'L'
                    if (l + 1 < 3) next[a][l + 1] = (next[a][l + 1] + dp[a][l]) % MOD;
                    // Add 'A'
                    if (a + 1 < 2) next[a + 1][0] = (next[a + 1][0] + dp[a][l]) % MOD;
                }
            }
            dp = next;
        }
        long sum = 0;
        for (int a = 0; a < 2; ++a)
            for (int l = 0; l < 3; ++l)
                sum = (sum + dp[a][l]) % MOD;
        return (int)sum;
    }
};
