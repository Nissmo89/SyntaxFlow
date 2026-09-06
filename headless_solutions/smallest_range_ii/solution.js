/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number}
 */
var smallestRangeII = function(nums, k) {
    nums.sort((a, b) => a - b);
    let n = nums.length;
    let res = nums[n - 1] - nums[0];
    
    for (let i = 0; i < n - 1; i++) {
        let high = Math.max(nums[n - 1] - k, nums[i] + k);
        let low = Math.min(nums[0] + k, nums[i + 1] - k);
        res = Math.min(res, high - low);
    }
    return res;
};
