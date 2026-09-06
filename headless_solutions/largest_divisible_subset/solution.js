var largestDivisibleSubset = function(nums) {
    nums.sort((a, b) => a - b);
    let n = nums.length, dp = new Array(n).fill(1), parent = new Array(n).fill(-1);
    let maxIdx = 0;
    for (let i = 1; i < n; i++) {
        for (let j = 0; j < i; j++) {
            if (nums[i] % nums[j] === 0 && dp[i] < dp[j] + 1) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
        if (dp[i] > dp[maxIdx]) maxIdx = i;
    }
    let res = [];
    for (let i = maxIdx; i !== -1; i = parent[i]) res.push(nums[i]);
    return res;
};
