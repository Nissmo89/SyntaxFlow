/**
 * Definition for a binary tree node.
 * function TreeNode(val, left, right) {
 *     this.val = (val===undefined ? 0 : val)
 *     this.left = (left===undefined ? null : left)
 *     this.right = (right===undefined ? null : right)
 * }
 */
/**
 * @param {number[]} nums
 * @return {TreeNode}
 */
var sortedArrayToBST = function(nums) {
    // Helper function to recursively build the BST
    function buildBST(left, right) {
        // Base case: if the sub-array is empty, return null
        if (left > right) {
            return null;
        }

        // Find the middle element to be the root of the current subtree
        // Using Math.floor for integer division
        let mid = Math.floor(left + (right - left) / 2);

        // Create a new TreeNode with the middle element's value
        let root = new TreeNode(nums[mid]);

        // Recursively build the left subtree using the left half of the array
        root.left = buildBST(left, mid - 1);

        // Recursively build the right subtree using the right half of the array
        root.right = buildBST(mid + 1, right);

        // Return the root of the current subtree
        return root;
    }

    // Handle empty input array (though constraints say nums.length >= 1)
    if (!nums || nums.length === 0) {
        return null;
    }

    // Start the recursive process with the entire array
    return buildBST(0, nums.length - 1);
};
