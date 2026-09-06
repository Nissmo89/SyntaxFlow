var profitableSchemes = function(n, minProfit, group, profit) {
    const MOD = 1e9 + 7;
    let dp = Array.from({ length: minProfit + 1 }, () => new Array(n + 1).fill(0));
    dp[0][0] = 1;
    
    for (let k = 0; k < group.length; k++) {
        let g = group[k], p = profit[k];
        for (let i = minProfit; i >= 0; i--) {
            for (let j = n - g; j >= 0; j--) {
                let nextP = Math.min(i + p, minProfit);
                dp[nextP][j + g] = (dp[nextP][j + g] + dp[i][j]) % MOD;
            }
        }
    }
    
    return dp[minProfit].reduce((a, b) => (a + b) % MOD, 0);
};
