var maxDepth = function(root) {
    if (!root) return 0;
    let maxLeft = maxDepth(root.left);
    let maxRight = maxDepth(root.right);
    return Math.max(maxLeft, maxRight) + 1;
};
