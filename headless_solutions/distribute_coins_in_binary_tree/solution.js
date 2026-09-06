var distributeCoins = function(root) {
    let moves = 0;
    function dfs(node) {
        if (!node) return 0;
        let left = dfs(node.left);
        let right = dfs(node.right);
        moves += Math.abs(left) + Math.abs(right);
        return node.val + left + right - 1;
    }
    dfs(root);
    return moves;
};
