class Solution {
public:
    string smallest = "{"; // '{' is greater than 'z'
    
    void dfs(TreeNode* node, string path) {
        if (!node) return;
        path = (char)('a' + node->val) + path;
        if (!node->left && !node->right) {
            if (path < smallest) smallest = path;
            return;
        }
        dfs(node->left, path);
        dfs(node->right, path);
    }
    
    string smallestFromLeaf(TreeNode* root) {
        dfs(root, "");
        return smallest;
    }
};
