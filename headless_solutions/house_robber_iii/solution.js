var rob = function(root) {
    const dfs = (node) => {
        if (!node) return [0, 0]; // [robbed, not_robbed]
        const left = dfs(node.left);
        const right = dfs(node.right);
        
        const rob = node.val + left[1] + right[1];
        const notRob = Math.max(...left) + Math.max(...right);
        
        return [rob, notRob];
    };
    return Math.max(...dfs(root));
};
