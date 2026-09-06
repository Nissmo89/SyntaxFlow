/**
 * Definition for a binary tree node.
 * function TreeNode(val, left, right) {
 *     this.val = (val===undefined ? 0 : val)
 *     this.left = (left===undefined ? null : left)
 *     this.right = (right===undefined ? null : right)
 * }
 */
/**
 * @param {number[]} preorder
 * @return {TreeNode}
 */
var bstFromPreorder = function(preorder) {
    let idx = 0;

    function build(bound) {
        if (idx === preorder.length || preorder[idx] > bound) {
            return null;
        }

        const val = preorder[idx++];
        const root = new TreeNode(val);

        root.left = build(val);
        root.right = build(bound);

        return root;
    }

    return build(Infinity);
};
