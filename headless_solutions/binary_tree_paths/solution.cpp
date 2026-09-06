/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result;
        if (root == nullptr) return result;
        
        dfs(root, to_string(root->val), result);
        return result;
    }
    
    void dfs(TreeNode* node, string path, vector<string>& result) {
        if (node->left == nullptr && node->right == nullptr) {
            result.push_back(path);
            return;
        }
        
        if (node->left != nullptr) {
            dfs(node->left, path + "->" + to_string(node->left->val), result);
        }
        
        if (node->right != nullptr) {
            dfs(node->right, path + "->" + to_string(node->right->val), result);
        }
    }
};
