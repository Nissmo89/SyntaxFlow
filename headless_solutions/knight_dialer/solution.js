var knightDialer = function(n) {
    if (n === 1) return 10;
    const MOD = 1e9 + 7;
    let dp = new Array(10).fill(1);
    for (let i = 1; i < n; i++) {
        let next = new Array(10).fill(0);
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
    return dp.reduce((a, b) => (a + b) % MOD, 0);
};
