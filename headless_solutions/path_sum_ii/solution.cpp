class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> result;
        vector<int> currentPath;
        dfs(root, targetSum, currentPath, result);
        return result;
    }
private:
    void dfs(TreeNode* node, int target, vector<int>& path, vector<vector<int>>& res) {
        if (!node) return;
        path.push_back(node->val);
        if (!node->left && !node->right && target == node->val) {
            res.push_back(path);
        }
        dfs(node->left, target - node->val, path, res);
        dfs(node->right, target - node->val, path, res);
        path.pop_back(); // Backtrack
    }
};
