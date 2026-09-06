/**
 * @param {number[]} stones
 * @return {number}
 */
var lastStoneWeightII = function(stones) {
    const sum = stones.reduce((a, b) => a + b, 0);
    const target = Math.floor(sum / 2);
    const dp = new Array(target + 1).fill(false);
    dp[0] = true;
    
    for (const stone of stones) {
        for (let i = target; i >= stone; i--) {
            if (dp[i - stone]) dp[i] = true;
        }
    }
    
    for (let i = target; i >= 0; i--) {
        if (dp[i]) return sum - 2 * i;
    }
    return 0;
};
