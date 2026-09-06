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
 * @param {number[]} inorder
 * @return {TreeNode}
 */
var buildTree = function(preorder, inorder) {
    const inMap = new Map();
    for (let i = 0; i < inorder.length; i++) {
        inMap.set(inorder[i], i);
    }

    let preIdx = 0;

    function build(inStart, inEnd) {
        if (inStart > inEnd) {
            return null;
        }

        const rootVal = preorder[preIdx++];
        const root = new TreeNode(rootVal);
        const inRootIdx = inMap.get(rootVal);

        root.left = build(inStart, inRootIdx - 1);
        root.right = build(inRootIdx + 1, inEnd);

        return root;
    }

    return build(0, inorder.length - 1);
};
