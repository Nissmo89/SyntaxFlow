var findNumberOfLIS = function(nums) {
    const n = nums.length;
    const lengths = new Array(n).fill(1);
    const counts = new Array(n).fill(1);
    let maxLen = 1;
    for (let i = 0; i < n; i++) {
        for (let j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                if (lengths[j] + 1 > lengths[i]) {
                    lengths[i] = lengths[j] + 1;
                    counts[i] = counts[j];
                } else if (lengths[j] + 1 === lengths[i]) {
                    counts[i] += counts[j];
                }
            }
        }
        maxLen = Math.max(maxLen, lengths[i]);
    }
    return counts.reduce((acc, count, i) => lengths[i] === maxLen ? acc + count : acc, 0);
};
