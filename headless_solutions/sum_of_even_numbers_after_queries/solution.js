/**
 * @param {number[]} nums
 * @param {number[][]} queries
 * @return {number[]}
 */
var sumEvenAfterQueries = function(nums, queries) {
    let evenSum = nums.reduce((acc, cur) => (cur % 2 === 0 ? acc + cur : acc), 0);
    const ans = [];
    for (const [val, idx] of queries) {
        if (nums[idx] % 2 === 0) evenSum -= nums[idx];
        nums[idx] += val;
        if (nums[idx] % 2 === 0) evenSum += nums[idx];
        ans.push(evenSum);
    }
    return ans;
};
