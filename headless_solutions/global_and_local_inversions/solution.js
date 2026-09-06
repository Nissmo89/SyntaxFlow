/**
 * @param {number[]} nums
 * @return {boolean}
 */
var isIdealPermutation = function(nums) {
    for (let i = 0; i < nums.length; i++) {
        if (Math.abs(nums[i] - i) > 1) return false;
    }
    return true;
};
