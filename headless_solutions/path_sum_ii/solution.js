var pathSum = function(root, targetSum) {
    const res = [];
    const dfs = (node, target, path) => {
        if (!node) return;
        path.push(node.val);
        if (!node.left && !node.right && target === node.val) {
            res.push([...path]);
        } else {
            dfs(node.left, target - node.val, path);
            dfs(node.right, target - node.val, path);
        }
        path.pop();
    };
    dfs(root, targetSum, []);
    return res;
};
