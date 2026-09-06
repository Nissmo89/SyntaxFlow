var smallestFromLeaf = function(root) {
    let smallest = "{";
    const dfs = (node, path) => {
        if (!node) return;
        path = String.fromCharCode(97 + node.val) + path;
        if (!node.left && !node.right) {
            if (path < smallest) smallest = path;
            return;
        }
        dfs(node.left, path);
        dfs(node.right, path);
    };
    dfs(root, "");
    return smallest;
};
