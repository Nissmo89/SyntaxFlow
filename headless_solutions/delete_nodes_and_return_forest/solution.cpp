class Solution {
    unordered_set<int> to_delete_set;
    vector<TreeNode*> forest;

    TreeNode* helper(TreeNode* node, bool is_root) {
        if (!node) return nullptr;
        bool deleted = to_delete_set.count(node->val);
        if (is_root && !deleted) forest.push_back(node);
        
        node->left = helper(node->left, deleted);
        node->right = helper(node->right, deleted);
        
        return deleted ? nullptr : node;
    }

public:
    vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
        for (int x : to_delete) to_delete_set.insert(x);
        helper(root, true);
        return forest;
    }
};
