var isSymmetric = function(root) {
    if (root === null) {
        return true;
    }
    return isMirror(root.left, root.right);

    function isMirror(node1, node2) {
        if (node1 === null && node2 === null) {
            return true;
        }
        if (node1 === null || node2 === null) {
            return false;
        }
        return node1.val === node2.val && isMirror(node1.left, node2.right) && isMirror(node1.right, node2.left);
    }
};
