class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> temp;
        backtrack(res, temp, nums, 0);
        return res;
    }
private:
    void backtrack(vector<vector<int>>& res, vector<int>& temp, vector<int>& nums, int start) {
        res.push_back(temp);
        for (int i = start; i < nums.size(); ++i) {
            temp.push_back(nums[i]);
            backtrack(res, temp, nums, i + 1);
            temp.pop_back();
        }
    }
};
