/**
 * @param {number[]} nums
 * @return {number}
 */
var findMaximumXOR = function(nums) {
    let max = 0, mask = 0;
    for (let i = 30; i >= 0; i--) {
        mask |= (1 << i);
        let set = new Set();
        for (let num of nums) set.add(num & mask);
        let candidate = max | (1 << i);
        for (let prefix of set) {
            if (set.has(prefix ^ candidate)) {
                max = candidate;
                break;
            }
        }
    }
    return max;
};
