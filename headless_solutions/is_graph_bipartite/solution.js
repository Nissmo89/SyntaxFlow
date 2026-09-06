var isBipartite = function(graph) {
    const n = graph.length;
    const colors = new Array(n).fill(0);
    
    const dfs = (node, color) => {
        colors[node] = color;
        for (let neighbor of graph[node]) {
            if (colors[neighbor] === color) return false;
            if (colors[neighbor] === 0 && !dfs(neighbor, -color)) return false;
        }
        return true;
    };
    
    for (let i = 0; i < n; i++) {
        if (colors[i] === 0 && !dfs(i, 1)) return false;
    }
    return true;
};
