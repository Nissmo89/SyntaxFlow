class Solution {
public:
    // Returns {robbed, not_robbed}
    pair<int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0};
        auto left = dfs(node->left);
        auto right = dfs(node->right);
        
        int rob = node->val + left.second + right.second;
        int not_rob = max(left.first, left.second) + max(right.first, right.second);
        
        return {rob, not_rob};
    }
    
    int rob(TreeNode* root) {
        auto res = dfs(root);
        return max(res.first, res.second);
    }
};
