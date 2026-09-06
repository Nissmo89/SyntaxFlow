/**
 * @param {number} k
 * @param {number} n
 * @return {number}
 */
var superEggDrop = function(k, n) {
    let dp = new Array(k + 1).fill(0);
    let m = 0;
    while (dp[k] < n) {
        m++;
        for (let i = k; i >= 1; i--) {
            dp[i] = dp[i] + dp[i - 1] + 1;
        }
    }
    return m;
};
