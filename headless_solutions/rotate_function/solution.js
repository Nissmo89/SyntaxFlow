/**
 * @param {number[]} nums
 * @return {number}
 */
var maxRotateFunction = function(nums) {
    const n = nums.length;
    let sum = 0;
    let f = 0;
    for (let i = 0; i < n; i++) {
        sum += nums[i];
        f += i * nums[i];
    }
    let maxF = f;
    for (let i = 1; i < n; i++) {
        f = f + sum - n * nums[n - i];
        maxF = Math.max(maxF, f);
    }
    return maxF;
};
