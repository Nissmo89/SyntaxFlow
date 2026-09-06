/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number}
 */
var findPairs = function(nums, k) {
    if (k < 0) return 0;
    const counts = new Map();
    for (const num of nums) {
        counts.set(num, (counts.get(num) || 0) + 1);
    }
    
    let pairs = 0;
    for (const [val, count] of counts.entries()) {
        if (k === 0) {
            if (count > 1) pairs++;
        } else {
            if (counts.has(val + k)) pairs++;
        }
    }
    return pairs;
};
