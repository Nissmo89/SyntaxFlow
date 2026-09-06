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
private:
    TreeNode* build(const vector<int>& preorder, int& index, int bound) {
        // Base case: out of bounds or current value violates BST upper bound constraint
        if (index >= preorder.size() || preorder[index] > bound) {
            return nullptr;
        }

        // Create the node and advance the pointer
        int val = preorder[index++];
        TreeNode* root = new TreeNode(val);

        // Subtrees must strictly respect their relative bounds
        root->left = build(preorder, index, val);
        root->right = build(preorder, index, bound);

        return root;
    }

public:
    TreeNode* bstFromPreorder(vector<int>& preorder) {
        int index = 0;
        return build(preorder, index, INT_MAX);
    }
};
