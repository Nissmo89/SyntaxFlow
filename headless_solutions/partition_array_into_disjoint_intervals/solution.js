/**
 * @param {number[]} nums
 * @return {number}
 */
var partitionDisjoint = function(nums) {
    let partitionIdx = 0;
    let maxLeft = nums[0];
    let currMax = nums[0];
    
    for (let i = 1; i < nums.length; i++) {
        currMax = Math.max(currMax, nums[i]);
        if (nums[i] < maxLeft) {
            partitionIdx = i;
            maxLeft = currMax;
        }
    }
    return partitionIdx + 1;
};
