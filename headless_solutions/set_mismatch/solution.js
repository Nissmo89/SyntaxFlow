/**
 * @param {number[]} nums
 * @return {number[]}
 */
var findErrorNums = function(nums) {
    let freq = {};
    let dup = -1;
    let missing = -1;
    
    for (let num of nums) {
        freq[num] = (freq[num] || 0) + 1;
        if (freq[num] == 2) {
            dup = num;
        }
    }
    
    let n = nums.length;
    let expectedSum = n * (n + 1) / 2;
    let actualSum = nums.reduce((a, b) => a + b, 0);
    
    missing = expectedSum - actualSum + dup;
    
    return [dup, missing];
};
