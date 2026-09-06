/**
 * @param {number[]} nums
 * @param {number} target
 * @return {number}
 */
var findTargetSumWays = function(nums, target) {
    const sum = nums.reduce((a, b) => a + b, 0);
    if (Math.abs(target) > sum || (target + sum) % 2 !== 0) return 0;
    
    const subsetTarget = (target + sum) / 2;
    const dp = new Array(subsetTarget + 1).fill(0);
    dp[0] = 1;
    
    for (const n of nums) {
        for (let i = subsetTarget; i >= n; i--) {
            dp[i] += dp[i - n];
        }
    }
    return dp[subsetTarget];
};
