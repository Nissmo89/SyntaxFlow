/**
 * @param {number[]} nums
 * @return {number}
 */
var numberOfArithmeticSlices = function(nums) {
    let n = nums.length;
    let total = 0;
    let dp = Array.from({ length: n }, () => new Map());
    for (let i = 0; i < n; i++) {
        for (let j = 0; j < i; j++) {
            let diff = nums[i] - nums[j];
            let countJ = dp[j].get(diff) || 0;
            dp[i].set(diff, (dp[i].get(diff) || 0) + countJ + 1);
            total += countJ;
        }
    }
    return total;
};
