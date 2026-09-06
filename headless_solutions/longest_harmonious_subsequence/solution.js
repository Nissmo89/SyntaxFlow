/**
 * @param {number[]} nums
 * @return {number}
 */
var findLHS = function(nums) {
    let count = {};
    for (let num of nums) {
        count[num] = (count[num] || 0) + 1;
    }
    let result = 0;
    for (let key in count) {
        if (count[key + 1]) {
            result = Math.max(result, count[key] + count[key + 1]);
        }
    }
    return result;
};
