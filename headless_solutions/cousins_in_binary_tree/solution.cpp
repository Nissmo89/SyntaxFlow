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
    bool isCousins(TreeNode* root, int x, int y) {
        TreeNode* xParent = nullptr;
        TreeNode* yParent = nullptr;
        int xDepth = -1, yDepth = -1;
        
        getDepthAndParent(root, x, y, 0, nullptr, xParent, yParent, xDepth, yDepth);
        
        return xDepth == yDepth && xParent != yParent;
    }
    
    void getDepthAndParent(TreeNode* root, int x, int y, int depth, TreeNode* parent, TreeNode*& xParent, TreeNode*& yParent, int& xDepth, int& yDepth) {
        if (root == nullptr) {
            return;
        }
        
        if (root->val == x) {
            xParent = parent;
            xDepth = depth;
        } else if (root->val == y) {
            yParent = parent;
            yDepth = depth;
        }
        
        getDepthAndParent(root->left, x, y, depth + 1, root, xParent, yParent, xDepth, yDepth);
        getDepthAndParent(root->right, x, y, depth + 1, root, xParent, yParent, xDepth, yDepth);
    }
};
