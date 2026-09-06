/**
 * @param {number} n
 * @return {number}
 */
var getMoneyAmount = function(n) {
    const dp = Array.from({ length: n + 2 }, () => Array(n + 2).fill(0));
    for (let len = 2; len <= n; len++) {
        for (let i = 1; i <= n - len + 1; i++) {
            let j = i + len - 1;
            dp[i][j] = Infinity;
            for (let k = i; k <= j; k++) {
                dp[i][j] = Math.min(dp[i][j], k + Math.max(dp[i][k - 1], dp[k + 1][j]));
            }
        }
    }
    return dp[1][n];
};
