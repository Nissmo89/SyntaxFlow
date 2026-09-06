/**
 * @param {number[]} nums
 * @return {number[]}
 */
var sortArrayByParityII = function(nums) {
    let i = 0;
    let j = 1;
    let n = nums.length;
    
    while (i < n && j < n) {
        while (i < n && nums[i] % 2 == 0) {
            i += 2;
        }
        while (j < n && nums[j] % 2 == 1) {
            j += 2;
        }
        if (i < n && j < n) {
            [nums[i], nums[j]] = [nums[j], nums[i]];
            i += 2;
            j += 2;
        }
    }
    
    return nums;
};
