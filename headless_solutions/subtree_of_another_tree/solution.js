var isSubtree = function(root, subRoot) {
    if (root === null) return false;
    if (isSame(root, subRoot)) return true;
    return isSubtree(root.left, subRoot) || isSubtree(root.right, subRoot);

    function isSame(s, t) {
        if (s === null && t === null) return true;
        if (s === null || t === null) return false;
        return (s.val === t.val) && isSame(s.left, t.left) && isSame(s.right, t.right);
    }
};
