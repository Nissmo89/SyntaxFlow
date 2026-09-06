/**
 * @param {number[]} nums
 * @param {number} left
 * @param {number} right
 * @return {number}
 */
var numSubarrayBoundedMax = function(nums, left, right) {
    let res = 0, lastInvalid = -1, lastCandidate = -1;
    for (let i = 0; i < nums.length; i++) {
        if (nums[i] > right) lastInvalid = i;
        if (nums[i] >= left) lastCandidate = i;
        res += (lastCandidate - lastInvalid);
    }
    return res;
};
