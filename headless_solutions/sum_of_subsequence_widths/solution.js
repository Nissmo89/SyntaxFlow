/**
 * @param {number[]} nums
 * @return {number}
 */
var sumSubseqWidths = function(nums) {
    nums.sort((a, b) => a - b);
    const mod = 1000000007n;
    let res = 0n;
    let p2 = 1n;
    const n = nums.length;
    
    for (let i = 0; i < n; i++) {
        res = (res + BigInt(nums[i]) * p2 - BigInt(nums[n - 1 - i]) * p2) % mod;
        p2 = (p2 * 2n) % mod;
    }
    return Number((res + mod) % mod);
};
