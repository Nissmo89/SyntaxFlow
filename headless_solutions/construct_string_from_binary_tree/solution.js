/**
 * @param {TreeNode} root
 * @return {string}
 */
var tree2str = function(root) {
    const buffer = [];

    function dfs(node) {
        if (!node) return;

        buffer.push(node.val.toString());

        if (node.left || node.right) {
            buffer.push('(');
            dfs(node.left);
            buffer.push(')');
        }

        if (node.right) {
            buffer.push('(');
            dfs(node.right);
            buffer.push(')');
        }
    }

    dfs(root);
    return buffer.join('');
};
