/**
 * @param {TreeNode} root
 * @return {number}
 */
var getMinimumDifference = function(root) {
    let min_diff = Infinity;
    let prev_val = -1;
    
    function inorderTraverse(node) {
        if (node.left) {
            inorderTraverse(node.left);
        }
        
        if (prev_val >= 0) {
            min_diff = Math.min(min_diff, node.val - prev_val);
        }
        
        prev_val = node.val;
        
        if (node.right) {
            inorderTraverse(node.right);
        }
    }
    
    inorderTraverse(root);
    
    return min_diff;
};
