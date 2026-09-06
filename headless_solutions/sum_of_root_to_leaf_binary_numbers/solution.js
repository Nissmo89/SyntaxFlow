/**
 * @param {TreeNode} root
 * @return {number}
 */
var sumRootToLeaf = function(root) {
    function dfs(root, val) {
        if (!root) return 0;
        val = val * 2 + root.val;
        return root.left == root.right && root.left == null && root.right == null ? val : dfs(root.left, val) + dfs(root.right, val);
    }

    return dfs(root, 0);
};
