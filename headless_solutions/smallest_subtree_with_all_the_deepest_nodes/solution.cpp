class Solution {
public:
    pair<int, TreeNode*> deep(TreeNode* root) {
        if (!root) return {0, nullptr};
        auto l = deep(root->left);
        auto r = deep(root->right);
        
        if (l.first > r.first) return {l.first + 1, l.second};
        if (r.first > l.first) return {r.first + 1, r.second};
        return {l.first + 1, root};
    }

    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        return deep(root).second;
    }
};
