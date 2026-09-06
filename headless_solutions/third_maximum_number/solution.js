/**
 * @param {number[]} nums
 * @return {number}
 */
var thirdMax = function(nums) {
    nums.sort((a, b) => b - a);
    let elemCounted = 1;
    let prevElem = nums[0];
    for (let i = 1; i < nums.length; i++) {
        if (nums[i] != prevElem) {
            elemCounted++;
            prevElem = nums[i];
            if (elemCounted == 3) {
                return nums[i];
            }
        }
    }
    return nums[0];
};
