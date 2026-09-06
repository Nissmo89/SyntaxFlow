var invertTree = function(root) {
    if (root === null) {
        return null;
    }
    invertTree(root.left);
    invertTree(root.right);
    let temp = root.left;
    root.left = root.right;
    root.right = temp;
    return root;
};
