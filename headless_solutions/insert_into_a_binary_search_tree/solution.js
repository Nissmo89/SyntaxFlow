var insertIntoBST = function(root, val) {
    if (!root) return new TreeNode(val);
    let curr = root;
    while (true) {
        if (val < curr.val) {
            if (!curr.left) { curr.left = new TreeNode(val); break; }
            curr = curr.left;
        } else {
            if (!curr.right) { curr.right = new TreeNode(val); break; }
            curr = curr.right;
        }
    }
    return root;
};
