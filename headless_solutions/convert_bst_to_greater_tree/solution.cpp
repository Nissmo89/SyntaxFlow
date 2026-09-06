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
    int cumulative_sum = 0; // Stores the sum of all nodes greater than the current node

    // Helper function to perform reverse in-order traversal
    void reverseInOrder(TreeNode* node) {
        // Base case: if the node is null, return
        if (!node) {
            return;
        }

        // 1. Traverse the right subtree first (nodes with greater values)
        reverseInOrder(node->right);

        // 2. Process the current node
        // Store the original value before modifying it
        int original_val = node->val;
        // Update the node's value: original_val + sum of all greater nodes
        node->val += cumulative_sum;
        // Update the cumulative_sum for subsequent nodes (which will be smaller)
        // Add the original value of the current node to the sum
        cumulative_sum += original_val;

        // 3. Traverse the left subtree (nodes with smaller values)
        reverseInOrder(node->left);
    }

public:
    TreeNode* convertBST(TreeNode* root) {
        // Reset cumulative_sum for each call to convertBST, in case it's called multiple times
        cumulative_sum = 0;
        // Start the reverse in-order traversal from the root
        reverseInOrder(root);
        // Return the modified root of the tree
        return root;
    }
};
