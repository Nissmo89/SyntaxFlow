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
    // Helper function to recursively build the BST
    TreeNode* buildBST(const std::vector<int>& nums, int left, int right) {
        // Base case: if the sub-array is empty, return nullptr
        if (left > right) {
            return nullptr;
        }

        // Find the middle element to be the root of the current subtree
        // Using (left + right) / 2 is fine for given constraints,
        // but left + (right - left) / 2 avoids potential overflow for very large indices
        int mid = left + (right - left) / 2;

        // Create a new TreeNode with the middle element's value
        TreeNode* root = new TreeNode(nums[mid]);

        // Recursively build the left subtree using the left half of the array
        root->left = buildBST(nums, left, mid - 1);

        // Recursively build the right subtree using the right half of the array
        root->right = buildBST(nums, mid + 1, right);

        // Return the root of the current subtree
        return root;
    }

    TreeNode* sortedArrayToBST(std::vector<int>& nums) {
        // Handle empty input array (though constraints say nums.length >= 1)
        if (nums.empty()) {
            return nullptr;
        }
        // Start the recursive process with the entire array
        return buildBST(nums, 0, nums.size() - 1);
    }
};
