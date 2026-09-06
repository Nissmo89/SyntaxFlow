var canPartitionKSubsets = function(nums, k) {
    const sum = nums.reduce((a, b) => a + b, 0);
    if (sum % k !== 0) return false;
    const target = sum / k, n = nums.length;
    const dp = new Array(1 << n).fill(-1);
    dp[0] = 0;
    for (let mask = 0; mask < (1 << n); mask++) {
        if (dp[mask] === -1) continue;
        for (let i = 0; i < n; i++) {
            if (!(mask & (1 << i)) && dp[mask] + nums[i] <= target) {
                dp[mask | (1 << i)] = (dp[mask] + nums[i]) % target;
            }
        }
    }
    return dp[(1 << n) - 1] === 0;
};
