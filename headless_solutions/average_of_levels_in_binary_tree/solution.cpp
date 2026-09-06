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
#include <queue> // Required for std::queue

class Solution {
public:
    std::vector<double> averageOfLevels(TreeNode* root) {
        std::vector<double> result;
        if (root == nullptr) {
            return result; // Handle empty tree
        }

        std::queue<TreeNode*> q;
        q.push(root); // Start BFS by adding the root

        while (!q.empty()) {
            int level_size = q.size(); // Number of nodes at the current level
            long long current_level_sum = 0; // Use long long to prevent overflow for sum of node values

            // Process all nodes at the current level
            for (int i = 0; i < level_size; ++i) {
                TreeNode* node = q.front(); // Get the front node
                q.pop();                   // Remove it from the queue

                current_level_sum += node->val; // Add its value to the sum

                // Add children to the queue for the next level
                if (node->left != nullptr) {
                    q.push(node->left);
                }
                if (node->right != nullptr) {
                    q.push(node->right);
                }
            }
            // Calculate average for the current level and add to result
            result.push_back(static_cast<double>(current_level_sum) / level_size);
        }

        return result;
    }
};
