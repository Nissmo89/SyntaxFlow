/**
 * @param {number[]} nums
 * @return {number}
 */
var arrayPairSum = function(nums) {
    // Step 1: Sort the array in non-decreasing order.
    // JavaScript's default sort converts elements to strings and sorts lexicographically.
    // For numeric sort, a custom comparator (a, b) => a - b is required.
    nums.sort((a, b) => a - b);

    let maximizedSum = 0;
    // Step 2: Iterate through the sorted array and sum every second element.
    // These elements (at indices 0, 2, 4, ...) will always be the minimum
    // of their respective pairs (nums[i], nums[i+1]).
    for (let i = 0; i < nums.length; i += 2) {
        maximizedSum += nums[i];
    }

    return maximizedSum;
};
