class Solution {
public:
    vector<int> maxScoreIndices(vector<int>& nums) {
        int n = nums.size();
        vector<int> prefix_sum(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix_sum[i + 1] = prefix_sum[i] + nums[i];
        }
        int max_score = 0;
        vector<int> max_indices;
        for (int i = 0; i <= n; i++) {
            if (prefix_sum[i] == 0) {
                max_score = n;
                max_indices.push_back(i);
            }
        }
        return max_indices;
    }
};
