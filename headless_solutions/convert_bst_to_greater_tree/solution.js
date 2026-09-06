/**
 * Definition for a binary tree node.
 * function TreeNode(val, left, right) {
 *     this.val = (val===undefined ? 0 : val)
 *     this.left = (left===undefined ? null : left)
 *     this.right = (right===undefined ? null : right)
 * }
 */
/**
 * @param {TreeNode} root
 * @return {TreeNode}
 */
var convertBST = function(root) {
    // Use an array to hold the cumulative sum so it can be mutated across recursive calls.
    // Alternatively, a class member variable could be used if this were part of a class.
    let cumulativeSum = [0]; // [0] is the initial sum

    /**
     * Helper function to perform reverse in-order traversal.
     * It modifies the tree in-place.
     * @param {TreeNode} node The current node being visited.
     */
    function reverseInOrder(node) {
        // Base case: if the node is null, return
        if (!node) {
            return;
        }

        // 1. Traverse the right subtree first (nodes with greater values)
        reverseInOrder(node.right);

        // 2. Process the current node
        // Store the original value before modifying it
        const originalVal = node.val;
        // Update the node's value: originalVal + sum of all greater nodes
        node.val += cumulativeSum[0];
        // Update the cumulativeSum for subsequent nodes (which will be smaller)
        // Add the original value of the current node to the sum
        cumulativeSum[0] += originalVal;

        // 3. Traverse the left subtree (nodes with smaller values)
        reverseInOrder(node.left);
    }

    // Start the reverse in-order traversal from the root
    reverseInOrder(root);
    // Return the modified root of the tree
    return root;
};
