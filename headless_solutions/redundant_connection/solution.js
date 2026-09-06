var findRedundantConnection = function(edges) {
    const parent = Array.from({ length: edges.length + 1 }, (_, i) => i);
    
    const find = (i) => {
        if (parent[i] === i) return i;
        return parent[i] = find(parent[i]);
    };
    
    for (const [u, v] of edges) {
        const rootU = find(u);
        const rootV = find(v);
        if (rootU === rootV) return [u, v];
        parent[rootU] = rootV;
    }
};
