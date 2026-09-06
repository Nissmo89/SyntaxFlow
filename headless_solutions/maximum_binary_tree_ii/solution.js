var insertIntoMaxTree = function(root, val) {
    if (root !== null && root.val > val) {
        root.right = insertIntoMaxTree(root.right, val);
        return root;
    }
    let node = new TreeNode(val);
    node.left = root;
    return node;
};
