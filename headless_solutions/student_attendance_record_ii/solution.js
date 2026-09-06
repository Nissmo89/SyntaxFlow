var checkRecord = function(n) {
    const MOD = 1000000007n;
    let dp = Array.from({ length: 2 }, () => new BigInt64Array(3));
    dp[0][0] = 1n;

    for (let i = 1; i <= n; i++) {
        let next = Array.from({ length: 2 }, () => new BigInt64Array(3));
        for (let a = 0; a < 2; a++) {
            for (let l = 0; l < 3; l++) {
                next[a][0] = (next[a][0] + dp[a][l]) % MOD;
                if (l + 1 < 3) next[a][l + 1] = (next[a][l + 1] + dp[a][l]) % MOD;
                if (a + 1 < 2) next[a + 1][0] = (next[a + 1][0] + dp[a][l]) % MOD;
            }
        }
        dp = next;
    }
    let sum = 0n;
    for (let a = 0; a < 2; a++)
        for (let l = 0; l < 3; l++)
            sum = (sum + dp[a][l]) % MOD;
    return Number(sum);
};
