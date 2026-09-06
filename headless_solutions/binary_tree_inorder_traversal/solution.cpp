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
#include <vector>
#include <stack> // Required for std::stack

class Solution {
public:
    std::vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> result;
        std::stack<TreeNode*> s; // Explicit stack for iterative traversal
        TreeNode* current = root; // Pointer to the current node

        // Loop continues as long as there are nodes to visit or nodes on the stack
        while (current != nullptr || !s.empty()) {
            // Phase 1: Go left as far as possible, pushing nodes onto the stack
            while (current != nullptr) {
                s.push(current);
                current = current->left;
            }

            // Phase 2: Current is null, meaning we've gone as far left as possible.
            // Pop the top node from the stack, process it, and then move to its right child.
            current = s.top(); // Get the node to visit (leftmost unvisited ancestor)
            s.pop();           // Remove it from the stack

            result.push_back(current->val); // Visit the node (add its value to result)

            current = current->right; // Move to the right subtree to continue traversal
        }

        return result;
    }
};
