var searchBST = function(root, val) {
    if (!root) return root;
    if (root.val == val) return root;
    else if (val < root.val) return searchBST(root.left, val);
    else return searchBST(root.right, val);
};
