class Solution {
public:
    int knightDialer(int n) {
        if (n == 1) return 10;
        long long MOD = 1e9 + 7;
        vector<long long> dp(10, 1);
        for (int i = 1; i < n; ++i) {
            vector<long long> next(10);
            next[0] = (dp[4] + dp[6]) % MOD;
            next[1] = (dp[6] + dp[8]) % MOD;
            next[2] = (dp[7] + dp[9]) % MOD;
            next[3] = (dp[4] + dp[8]) % MOD;
            next[4] = (dp[0] + dp[3] + dp[9]) % MOD;
            next[5] = 0;
            next[6] = (dp[0] + dp[1] + dp[7]) % MOD;
            next[7] = (dp[2] + dp[6]) % MOD;
            next[8] = (dp[1] + dp[3]) % MOD;
            next[9] = (dp[2] + dp[4]) % MOD;
            dp = next;
        }
        long long total = 0;
        for (long long x : dp) total = (total + x) % MOD;
        return (int)total;
    }
};
