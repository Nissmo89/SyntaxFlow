/**
 * @param {number[]} nums
 * @return {number}
 */
var longestArithSeqLength = function(nums) {
    const n = nums.length;
    if (n <= 2) return n;
    let res = 2;
    // Array of Maps to store diff -> length
    const dp = Array.from({ length: n }, () => new Map());
    for (let i = 0; i < n; i++) {
        for (let j = 0; j < i; j++) {
            const diff = nums[i] - nums[j];
            const len = (dp[j].get(diff) || 1) + 1;
            dp[i].set(diff, len);
            res = Math.max(res, len);
        }
    }
    return res;
};
