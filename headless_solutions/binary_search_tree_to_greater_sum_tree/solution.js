var bstToGst = function(root) {
    let pre = 0;
    function helper(root) {
        if (root) {
            helper(root.right);
            pre += root.val;
            root.val = pre;
            helper(root.left);
        }
    }
    helper(root);
    return root;
};
