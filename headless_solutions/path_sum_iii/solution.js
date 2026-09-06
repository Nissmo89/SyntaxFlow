var pathSum = function(root, targetSum) {
    let count = 0;
    let map = new Map([[0, 1]]);
    
    const dfs = (node, curr) => {
        if (!node) return;
        curr += node.val;
        count += map.get(curr - targetSum) || 0;
        map.set(curr, (map.get(curr) || 0) + 1);
        dfs(node.left, curr);
        dfs(node.right, curr);
        map.set(curr, map.get(curr) - 1); // Backtrack
    };
    
    dfs(root, 0);
    return count;
};
