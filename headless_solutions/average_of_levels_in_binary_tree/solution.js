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
var averageOfLevels = function(root) {
    const result = [];
    if (!root) {
        return result; // Handle empty tree
    }

    const q = [root]; // Use an array as a queue (push for enqueue, shift for dequeue)

    while (q.length > 0) {
        const level_size = q.length; // Number of nodes at the current level
        let current_level_sum = 0; // JavaScript numbers are 64-bit floats, sufficient for sums up to 2^53

        // Process all nodes at the current level
        for (let i = 0; i < level_size; ++i) {
            const node = q.shift(); // Dequeue from the front
            current_level_sum += node.val; // Add its value to the sum

            // Add children to the queue for the next level
            if (node.left) {
                q.push(node.left);
            }
            if (node.right) {
                q.push(node.right);
            }
        }
        // Calculate average for the current level and add to result
        result.push(current_level_sum / level_size);
    }

    return result;
};
