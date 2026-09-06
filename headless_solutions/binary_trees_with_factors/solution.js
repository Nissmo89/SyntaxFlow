/**
 * @param {number[]} arr
 * @return {number}
 */
var numFactoredBinaryTrees = function(arr) {
    let mod = 10**9 + 7;
    arr.sort((a, b) => a - b);
    let dp = {};
    for (let a of arr) {
        dp[a] = 1;
        for (let b of arr.slice(0, a)) {
            if (a % b == 0) {
                let c = a / b;
                if (c in dp) {
                    dp[a] = (dp[a] + dp[b] * dp[c]) % mod;
                }
            }
        }
    }
    return arr.reduce((sum, value) => (sum + dp[value]) % mod, 0);
};
