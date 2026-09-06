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
 * @return {number}
 */
var countNodes = function(root) {
    function height(node) {
        return node === null ? -1 : 1 + height(node.left);
    }

    if (root === null) return 0;
    let h = height(root);
    return h < 0 ? 0 :
           height(root.right) === h - 1 ? (1 << h) + countNodes(root.right)
                                        : (1 << h - 1) + countNodes(root.left);
};
