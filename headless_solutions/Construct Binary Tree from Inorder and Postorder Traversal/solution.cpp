#include <vector>
#include <map>
#include <algorithm> // For std::reverse if needed, but not for this approach

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    // Map to store value -> index for inorder traversal for O(1) lookup
    std::map<int, int> inorderMap;
    
    TreeNode* buildTree(std::vector<int>& inorder, std::vector<int>& postorder) {
        // Populate the inorderMap
        for (int i = 0; i < inorder.size(); ++i) {
            inorderMap[inorder[i]] = i;
        }
        
        // Call the recursive helper function
        return build(inorder, postorder, 
                     0, inorder.size() - 1, 
                     0, postorder.size() - 1);
    }
    
    // Recursive helper function
    TreeNode* build(const std::vector<int>& inorder, const std::vector<int>& postorder,
                    int inStart, int inEnd,
                    int postStart, int postEnd) {
        
        // Base case: if the segment is invalid, return nullptr
        if (inStart > inEnd || postStart > postEnd) {
            return nullptr;
        }
        
        // The root of the current subtree is the last element in the postorder segment
        int rootVal = postorder[postEnd];
        TreeNode* root = new TreeNode(rootVal);
        
        // Find the root's index in the inorder traversal using the map
        int rootIdxInInorder = inorderMap[rootVal];
        
        // Calculate the number of elements in the left subtree
        int numLeft = rootIdxInInorder - inStart;
        
        // Recursively build the right subtree
        // Inorder range: from (rootIdxInInorder + 1) to inEnd
        // Postorder range: from (postStart + numLeft) to (postEnd - 1)
        // The right subtree elements in postorder are after the left subtree elements
        // and before the current root.
        root->right = build(inorder, postorder, 
                            rootIdxInInorder + 1, inEnd, 
                            postStart + numLeft, postEnd - 1);
        
        // Recursively build the left subtree
        // Inorder range: from inStart to (rootIdxInInorder - 1)
        // Postorder range: from postStart to (postStart + numLeft - 1)
        // The left subtree elements in postorder are at the very beginning of the segment.
        root->left = build(inorder, postorder, 
                           inStart, rootIdxInInorder - 1, 
                           postStart, postStart + numLeft - 1);
        
        return root;
    }
};
