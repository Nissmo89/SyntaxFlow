var subtreeWithAllDeepest = function(root) {
    const dfs = (node) => {
        if (!node) return { depth: 0, node: null };
        const left = dfs(node.left);
        const right = dfs(node.right);
        
        if (left.depth > right.depth) return { depth: left.depth + 1, node: left.node };
        if (right.depth > left.depth) return { depth: right.depth + 1, node: right.node };
        return { depth: left.depth + 1, node: node };
    };
    return dfs(root).node;
};
