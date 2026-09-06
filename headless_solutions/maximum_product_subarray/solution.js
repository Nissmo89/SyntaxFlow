/**
 * @param {number[]} nums
 * @return {number}
 */
var maxProduct = function(nums) {
    let globalMax = nums[0];
    let curMax = nums[0];
    let curMin = nums[0];
    
    for (let i = 1; i < nums.length; i++) {
        if (nums[i] < 0) [curMax, curMin] = [curMin, curMax];
        
        curMax = Math.max(nums[i], curMax * nums[i]);
        curMin = Math.min(nums[i], curMin * nums[i]);
        
        globalMax = Math.max(globalMax, curMax);
    }
    return globalMax;
};
