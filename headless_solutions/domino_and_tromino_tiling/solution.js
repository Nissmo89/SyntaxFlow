var numTilings = function(n) {
    if (n <= 2) return n;
    if (n === 3) return 5;
    const MOD = 1e9 + 7;
    let dp = new BigInt64Array(n + 1);
    dp[1] = 1n; dp[2] = 2n; dp[3] = 5n;
    for (let i = 4; i <= n; i++) {
        dp[i] = (2n * dp[i - 1] + dp[i - 3]) % BigInt(MOD);
    }
    return Number(dp[n]);
};
