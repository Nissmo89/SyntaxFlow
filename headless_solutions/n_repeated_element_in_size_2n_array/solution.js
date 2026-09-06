/**
 * @param {number[]} nums
 * @return {number}
 */
var repeatedNTimes = function(nums) {
    let count = {};
    for (let num of nums) {
        count[num] = (count[num] || 0) + 1;
    }
    for (let num in count) {
        if (count[num] > 1) {
            return parseInt(num);
        }
    }
    return -1; // This should never happen
};
