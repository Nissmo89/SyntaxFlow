var isUnivalTree = function(root) {
    if (!root) return true;
    let val = root.val;
    return dfs(root, val);

    function dfs(node, val) {
        if (!node) return true;
        if (node.val != val) return false;
        return dfs(node.left, val) && dfs(node.right, val);
    }
};
