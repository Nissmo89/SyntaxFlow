// C++ implementation matching signature
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> ans;
        vector<int> path;
        sort(candidates.begin(), candidates.end());
        backtrack(candidates, target, 0, 0, path, ans);
        return ans;
    }

private:
    void backtrack(const vector<int>& cand, int target, int idx,
                   int curSum, vector<int>& path, vector<vector<int>>& ans) {
        if (curSum == target) {
            ans.push_back(path);
            return;
        }
        if (curSum > target) return;

        for (int i = idx; i < cand.size(); ++i) {
            if (curSum + cand[i] > target) break;          // pruning
            path.push_back(cand[i]);
            backtrack(cand, target, i, curSum + cand[i], path, ans); // i allows reuse
            path.pop_back();
        }
    }
};
