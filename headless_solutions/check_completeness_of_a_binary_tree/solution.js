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
 * @return {boolean}
 */
var isCompleteTree = function(root) {
    const queue = [root];
    let seenNull = false;
    
    while (queue.length) {
        const cur = queue.shift();   // dequeue
        if (cur === null) {
            seenNull = true;
        } else {
            if (seenNull) return false;   // non‑null after a gap
            queue.push(cur.left);
            queue.push(cur.right);
        }
    }
    return true;
};
