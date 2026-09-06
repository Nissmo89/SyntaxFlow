/**
 * @param {number[]} nums
 * @return {boolean}
 */
var containsDuplicate = function(nums) {
    let uniqueElements = new Set();
    for (let num of nums) {
        if (uniqueElements.has(num)) {
            return true;
        }
        uniqueElements.add(num);
    }
    return false;
};
