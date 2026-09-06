/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number}
 */
var subarraysDivByK = function(nums, k) {
    let count = new Array(k).fill(0);
    count[0] = 1;
    let prefixSum = 0, result = 0;
    for (let x of nums) {
        prefixSum = (prefixSum + x) % k;
        if (prefixSum < 0) prefixSum += k;
        result += count[prefixSum];
        count[prefixSum]++;
    }
    return result;
};
