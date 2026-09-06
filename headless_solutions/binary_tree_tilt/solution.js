var findTilt = function(root) {
    let result = 0;
    
    function postOrder(node) {
        if (!node) return 0;
        
        let left = postOrder(node.left);
        let right = postOrder(node.right);
        
        result += Math.abs(left - right);
        
        return left + right + node.val;
    }
    
    postOrder(root);
    return result;
};
