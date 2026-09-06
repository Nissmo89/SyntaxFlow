var strangePrinter = function(s) {
    let t = "";
    for (let char of s) if (t[t.length - 1] !== char) t += char;
    const n = t.length;
    const dp = Array.from({ length: n }, () => Array(n).fill(0));
    for (let i = n - 1; i >= 0; i--) {
        dp[i][i] = 1;
        for (let j = i + 1; j < n; j++) {
            dp[i][j] = dp[i][j - 1] + 1;
            for (let k = i; k < j; k++) {
                if (t[k] === t[j])
                    dp[i][j] = Math.min(dp[i][j], dp[i][k] + (k + 1 <= j - 1 ? dp[k + 1][j - 1] : 0));
            }
        }
    }
    return dp[0][n - 1];
};
