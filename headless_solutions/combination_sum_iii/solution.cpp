class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> ans;
        combination(ans, {}, k, 1, n);
        return ans;
    }

    void combination(vector<vector<int>>& ans, vector<int>& comb, int k, int start, int n) {
        if (comb.size() == k && n == 0) {
            ans.push_back(comb);
            return;
        }
        for (int i = start; i <= 9; i++) {
            comb.push_back(i);
            combination(ans, comb, k, i+1, n-i);
            comb.pop_back();
        }
    }
};
