#include <vector>
#include <unordered_map>

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
    std::unordered_map<int, int> inMap;
    int preIdx = 0;

    TreeNode* build(const std::vector<int>& preorder, int inStart, int inEnd) {
        if (inStart > inEnd) {
            return nullptr;
        }

        // Pick current root from preorder traversal using preIdx
        int rootVal = preorder[preIdx++];
        TreeNode* root = new TreeNode(rootVal);

        // Fetch root position in inorder array
        int inRootIdx = inMap[rootVal];

        // Build left and right subtrees recursively
        root->left = build(preorder, inStart, inRootIdx - 1);
        root->right = build(preorder, inRootIdx + 1, inEnd);

        return root;
    }

public:
    TreeNode* buildTree(std::vector<int>& preorder, std::vector<int>& inorder) {
        preIdx = 0;
        inMap.clear();
        
        // Build map from value to index for O(1) lookups
        for (int i = 0; i < inorder.size(); ++i) {
            inMap[inorder[i]] = i;
        }

        return build(preorder, 0, inorder.size() - 1);
    }
};
