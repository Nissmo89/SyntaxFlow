var numMusicPlaylists = function(n, goal, k) {
    const MOD = 1000000007n;
    let dp = Array.from({ length: goal + 1 }, () => new BigInt64Array(n + 1));
    dp[0][0] = 1n;
    for (let i = 1; i <= goal; i++) {
        for (let j = 1; j <= n; j++) {
            dp[i][j] = (dp[i - 1][j - 1] * BigInt(n - j + 1)) % MOD;
            if (j > k) {
                dp[i][j] = (dp[i][j] + dp[i - 1][j] * BigInt(j - k)) % MOD;
            }
        }
    }
    return Number(dp[goal][n]);
};
