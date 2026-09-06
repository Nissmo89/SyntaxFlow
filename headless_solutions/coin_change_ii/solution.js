/**
 * @param {number} amount
 * @param {number[]} coins
 * @return {number}
 */
var change = function(amount, coins) {
    // dp[i] = number of ways to make amount i
    const dp = new Array(amount + 1).fill(0);
    dp[0] = 1; // one way to make amount 0

    for (const coin of coins) {
        // iterate amounts from coin to amount (inclusive)
        for (let amt = coin; amt <= amount; ++amt) {
            dp[amt] += dp[amt - coin];
        }
    }

    return dp[amount];
};
