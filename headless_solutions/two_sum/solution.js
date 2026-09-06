/**
 * @param {number[]} nums
 * @param {number} target
 * @return {number[]}
 */
var twoSum = function(nums, target) {
    let numToIndex = {};
    for (let i = 0; i < nums.length; i++) {
        let complement = target - nums[i];
        if (complement in numToIndex) {
            return [numToIndex[complement], i];
        }
        numToIndex[nums[i]] = i;
    }
    return [];
};
