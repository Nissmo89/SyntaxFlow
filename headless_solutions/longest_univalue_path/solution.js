var longestUnivaluePath = function(root) {
    let maxPath = 0;
    const dfs = (node) => {
        if (!node) return 0;
        let left = dfs(node.left);
        let right = dfs(node.right);
        let arrowLeft = (node.left && node.left.val === node.val) ? left + 1 : 0;
        let arrowRight = (node.right && node.right.val === node.val) ? right + 1 : 0;
        maxPath = Math.max(maxPath, arrowLeft + arrowRight);
        return Math.max(arrowLeft, arrowRight);
    };
    dfs(root);
    return maxPath;
};
