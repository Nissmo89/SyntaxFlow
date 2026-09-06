/**
 * @param {number[]} nums
 * @return {number}
 */
var pivotIndex = function(nums) {
    let prefixSum = 0;
    for (let num of nums) {
        prefixSum += num;
    }
    
    let leftSum = 0;
    for (let i = 0; i < nums.length; i++) {
        if (leftSum == prefixSum - leftSum - nums[i]) {
            return i;
        }
        leftSum += nums[i];
    }
    
    return -1;
};
