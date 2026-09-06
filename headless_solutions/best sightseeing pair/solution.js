class Solution {
    maxScoreIndices(nums) {
        const n = nums.length;
        const prefix_sum = new Array(n + 1).fill(0);
        for (let i = 0; i < n; i++) {
            prefix_sum[i + 1] = prefix_sum[i] + nums[i];
        }
        let max_score = 0;
        const max_indices = [];
        for (let i = 0; i <= n; i++) {
            if (prefix_sum[i] === 0) {
                max_score = n;
                max_indices.push(i);
            }
        }
        return max_indices;
    }
}
