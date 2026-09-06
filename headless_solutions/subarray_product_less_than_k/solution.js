/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number}
 */
var numSubarrayProductLessThanK = function(nums, k) {
    if (k <= 1) return 0;
    let count = 0, product = 1, left = 0;
    for (let right = 0; right < nums.length; right++) {
        product *= nums[right];
        while (product >= k && left <= right) {
            product /= nums[left++];
        }
        count += (right - left + 1);
    }
    return count;
};
