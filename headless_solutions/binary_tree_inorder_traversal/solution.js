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
 * @return {number[]}
 */
var inorderTraversal = function(root) {
    const result = [];
    const stack = []; // Explicit stack for iterative traversal
    let current = root; // Pointer to the current node

    // Loop continues as long as there are nodes to visit or nodes on the stack
    while (current !== null || stack.length > 0) {
        // Phase 1: Go left as far as possible, pushing nodes onto the stack
        while (current !== null) {
            stack.push(current);
            current = current.left;
        }

        // Phase 2: Current is null, meaning we've gone as far left as possible.
        // Pop the top node from the stack, process it, and then move to its right child.
        current = stack.pop(); // Get the node to visit (leftmost unvisited ancestor)
        
        result.push(current.val); // Visit the node (add its value to result)
        
        current = current.right; // Move to the right subtree to continue traversal
    }

    return result;
};
