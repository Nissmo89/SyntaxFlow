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
 * @return {string[]}
 */
var binaryTreePaths = function(root) {
    let result = [];
    if (!root) return result;
    
    dfs(root, root.val.toString(), result);
    return result;
};

function dfs(node, path, result) {
    if (!node.left && !node.right) {
        result.push(path);
        return;
    }
    
    if (node.left) {
        dfs(node.left, path + "->" + node.left.val, result);
    }
    
    if (node.right) {
        dfs(node.right, path + "->" + node.right.val, result);
    }
}
