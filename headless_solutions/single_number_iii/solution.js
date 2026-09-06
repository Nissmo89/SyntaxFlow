/**
 * @param {number[]} nums
 * @return {number[]}
 */
var singleNumber = function(nums) {
    let xorSum = 0;
    for (let num of nums) xorSum ^= num;
    
    // In JS, bitwise operations treat numbers as 32-bit signed integers
    let diff = xorSum & -xorSum;
    
    let a = 0, b = 0;
    for (let num of nums) {
        if ((num & diff) !== 0) a ^= num;
        else b ^= num;
    }
    return [a, b];
};
