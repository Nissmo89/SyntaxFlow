class Solution {
public:
    int maxAncestorDiff(TreeNode* root, int mn = 100000, int mx = 0) {
        if (!root) return mx - mn;
        mn = min(mn, root->val);
        mx = max(mx, root->val);
        return max(maxAncestorDiff(root->left, mn, mx), 
                   maxAncestorDiff(root->right, mn, mx));
    }
};
