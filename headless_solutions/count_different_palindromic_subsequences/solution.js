/**
 * @param {string} s
 * @return {number}
 */
var countPalindromicSubsequences = function(s) {
    const n = s.length;
    const MOD = 1e9 + 7;
    const dp = Array.from({ length: n }, () => new Float64Array(n));

    for (let i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    for (let len = 2; len <= n; len++) {
        for (let i = 0; i <= n - len; i++) {
            let j = i + len - 1;
            if (s[i] === s[j]) {
                let low = i + 1;
                let high = j - 1;
                while (low <= high && s[low]!== s[i]) low++;
                while (low <= high && s[high]!== s[i]) high--;

                if (low > high) {
                    dp[i][j] = dp[i + 1][j - 1] * 2 + 2;
                } else if (low === high) {
                    dp[i][j] = dp[i + 1][j - 1] * 2 + 1;
                } else {
                    dp[i][j] = dp[i + 1][j - 1] * 2 - dp[low + 1][high - 1];
                }
            } else {
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1];
            }
            dp[i][j] = (dp[i][j] + MOD) % MOD;
        }
    }

    return Math.round(dp[0][n - 1]);
};
