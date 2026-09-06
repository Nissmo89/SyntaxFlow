var maxAncestorDiff = function(root, min = Infinity, max = -Infinity) {
    if (!root) return max - min;
    min = Math.min(min, root.val);
    max = Math.max(max, root.val);
    return Math.max(maxAncestorDiff(root.left, min, max), 
                    maxAncestorDiff(root.right, min, max));
};
