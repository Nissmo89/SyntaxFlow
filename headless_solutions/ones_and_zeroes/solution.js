var findMaxForm = function(strs, m, n) {
    let dp = Array.from({ length: m + 1 }, () => new Int32Array(n + 1));
    for (const s of strs) {
        let zeros = 0, ones = 0;
        for (const char of s) char === '0' ? zeros++ : ones++;
        for (let i = m; i >= zeros; i--) {
            for (let j = n; j >= ones; j--) {
                dp[i][j] = Math.max(dp[i][j], dp[i - zeros][j - ones] + 1);
            }
        }
    }
    return dp[m][n];
};
