/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number}
 */
var smallestRangeI = function(nums, k) {
    let M = Math.max(...nums), m = Math.min(...nums);
    let diff, extension = 2 * k;
    if (diff = M - m <= extension) {
        return 0;
    } else {
        return diff - extension;
    }
};
