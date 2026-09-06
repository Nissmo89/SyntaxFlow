var findUnsortedSubarray = function(nums) {
    let n = nums.length;
    let start = -1, end = -2;
    let min = nums[n - 1], max = nums[0];
    
    for (let i = 1; i < n; i++) {
        max = Math.max(max, nums[i]);
        min = Math.min(min, nums[n - 1 - i]);
        
        if (nums[i] < max) end = i;
        if (nums[n - 1 - i] > min) start = n - 1 - i;
    }
    return end - start + 1;
};
