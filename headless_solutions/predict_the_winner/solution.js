var predictTheWinner = function(nums) {
    const n = nums.length;
    const dp = Array.from({ length: n }, () => new Array(n).fill(0));
    for (let i = 0; i < n; i++) dp[i][i] = nums[i];
    for (let len = 1; len < n; len++) {
        for (let i = 0; i < n - len; i++) {
            let j = i + len;
            dp[i][j] = Math.max(nums[i] - dp[i + 1][j], nums[j] - dp[i][j - 1]);
        }
    }
    return dp[0][n - 1] >= 0;
};
