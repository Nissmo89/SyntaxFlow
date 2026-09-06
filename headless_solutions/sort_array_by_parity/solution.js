/**
 * @param {number[]} nums
 * @return {number[]}
 */
var sortArrayByParity = function(nums) {
    let beg = 0, end = nums.length - 1;
    while (beg <= end) {
        if (nums[beg] % 2 == 0) {
            beg++;
        } else {
            [nums[beg], nums[end]] = [nums[end], nums[beg]];
            end--;
        }
    }
    return nums;
};
