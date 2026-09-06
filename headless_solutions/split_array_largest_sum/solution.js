var splitArray = function(nums, k) {
    let left = Math.max(...nums);
    let right = nums.reduce((a, b) => a + b, 0);
    
    const canSplit = (target) => {
        let count = 1, sum = 0;
        for (let num of nums) {
            sum += num;
            if (sum > target) {
                sum = num;
                count++;
            }
        }
        return count <= k;
    };
    
    while (left < right) {
        let mid = Math.floor((left + right) / 2);
        if (canSplit(mid)) right = mid;
        else left = mid + 1;
    }
    return left;
};
