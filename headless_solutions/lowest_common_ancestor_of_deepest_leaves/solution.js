var lcaDeepestLeaves = function(root) {
    const dfs = (node) => {
        if (!node) return { depth: 0, lca: null };
        
        const left = dfs(node.left);
        const right = dfs(node.right);
        
        if (left.depth > right.depth) return { depth: left.depth + 1, lca: left.lca };
        if (right.depth > left.depth) return { depth: right.depth + 1, lca: right.lca };
        return { depth: left.depth + 1, lca: node };
    };
    
    return dfs(root).lca;
};
