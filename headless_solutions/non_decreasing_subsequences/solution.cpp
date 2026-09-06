class Solution {
public:
    void backtrack(vector<int>& nums, int start, vector<int>& path, vector<vector<int>>& res) {
        if (path.size() >= 2) res.push_back(path);
        unordered_set<int> used;
        for (int i = start; i < nums.size(); ++i) {
            if (used.count(nums[i])) continue;
            if (path.empty() || nums[i] >= path.back()) {
                used.insert(nums[i]);
                path.push_back(nums[i]);
                backtrack(nums, i + 1, path, res);
                path.pop_back();
            }
        }
    }
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        backtrack(nums, 0, path, res);
        return res;
    }
};
