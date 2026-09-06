class Solution {
    int maxPath = 0;
    int dfs(TreeNode* node) {
        if (!node) return 0;
        int left = dfs(node->left);
        int right = dfs(node->right);
        int arrowLeft = 0, arrowRight = 0;
        if (node->left && node->left->val == node->val) arrowLeft = left + 1;
        if (node->right && node->right->val == node->val) arrowRight = right + 1;
        maxPath = max(maxPath, arrowLeft + arrowRight);
        return max(arrowLeft, arrowRight);
    }
public:
    int longestUnivaluePath(TreeNode* root) {
        maxPath = 0;
        dfs(root);
        return maxPath;
    }
};
