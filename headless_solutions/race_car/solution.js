/**
 * @param {number} target
 * @return {number}
 */
var racecar = function(target) {
    const dp = new Array(target + 1).fill(0);
    const solve = (t) => {
        if (dp[t] > 0) return dp[t];
        let n = Math.floor(Math.log2(t)) + 1;
        if ((1 << n) - 1 === t) return dp[t] = n;
        
        dp[t] = solve((1 << n) - 1 - t) + n + 1;
        for (let m = 0; m < n - 1; m++) {
            dp[t] = Math.min(dp[t], solve(t - (1 << (n - 1)) + (1 << m)) + n + m + 1);
        }
        return dp[t];
    };
    return solve(target);
};
