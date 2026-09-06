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
 * @param {number} x
 * @param {number} y
 * @return {boolean}
 */
var isCousins = function(root, x, y) {
    let xParent = null;
    let yParent = null;
    let xDepth = -1;
    let yDepth = -1;
    
    getDepthAndParent(root, x, y, 0, null, (parent, depth) => {
        if (parent.val === x) {
            xParent = parent;
            xDepth = depth;
        } else if (parent.val === y) {
            yParent = parent;
            yDepth = depth;
        }
    });
    
    return xDepth === yDepth && xParent !== yParent;
};

function getDepthAndParent(root, x, y, depth, parent, callback) {
    if (root === null) {
        return;
    }
    
    callback(root, depth);
    
    getDepthAndParent(root.left, x, y, depth + 1, root, callback);
    getDepthAndParent(root.right, x, y, depth + 1, root, callback);
}
