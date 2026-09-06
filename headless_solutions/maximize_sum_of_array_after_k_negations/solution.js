/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number}
 */
var largestSumAfterKNegations = function(nums, k) {
    nums.sort((a, b) => a - b);
    let i = 0;
    while (i < nums.length && i < k && nums[i] < 0) {
        nums[i] = -nums[i];
        i++;
    }
    return nums.reduce((a, b) => a + b, 0) - (k - i) % 2 * Math.min(...nums) * 2;
};
