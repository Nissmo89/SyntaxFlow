var maximalSquare = function(matrix) {
    const m = matrix.length, n = matrix[0].length;
    let dp = new Array(n + 1).fill(0);
    let maxSide = 0, prev = 0;
    for (let i = 1; i <= m; i++) {
        for (let j = 1; j <= n; j++) {
            let temp = dp[j];
            if (matrix[i - 1][j - 1] === '1') {
                dp[j] = Math.min(dp[j], dp[j - 1], prev) + 1;
                maxSide = Math.max(maxSide, dp[j]);
            } else {
                dp[j] = 0;
            }
            prev = temp;
        }
    }
    return maxSide * maxSide;
};
