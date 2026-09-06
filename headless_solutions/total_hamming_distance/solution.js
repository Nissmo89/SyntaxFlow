/**
 * @param {number[]} nums
 * @return {number}
 */
var totalHammingDistance = function(nums) {
    let total = 0;
    const n = nums.length;
    for (let i = 0; i < 32; i++) {
        let count = 0;
        for (let num of nums) {
            if ((num >> i) & 1) count++;
        }
        total += count * (n - count);
    }
    return total;
};
