class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vector<vector<int>> result;
        vector<int> path;
        backtrack(candidates, target, 0, 0, path, result);
        return result;
    }

private:
    void backtrack(const vector<int>& cand, int remain, int start,
                   int pathSum, vector<int>& path, vector<vector<int>>& result) {
        if (pathSum == remain) {
            result.push_back(path);
            return;
        }
        if (pathSum > remain) {
            return;
        }
        for (int i = start; i < cand.size(); ++i) {
            if (i > start && cand[i] == cand[i - 1]) continue; // skip duplicates
            path.push_back(cand[i]);
            backtrack(cand, remain, i + 1, pathSum + cand[i], path, result);
            path.pop_back();
        }
    }
};
