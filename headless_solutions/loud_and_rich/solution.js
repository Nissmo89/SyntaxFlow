var loudAndRich = function(richer, quiet) {
    const n = quiet.length;
    const adj = Array.from({ length: n }, () => []);
    for (const [u, v] of richer) adj[v].push(u);
    
    const answer = new Array(n).fill(-1);
    
    const dfs = (node) => {
        if (answer[node] !== -1) return answer[node];
        answer[node] = node;
        for (const neighbor of adj[node]) {
            const candidate = dfs(neighbor);
            if (quiet[candidate] < quiet[answer[node]]) {
                answer[node] = candidate;
            }
        }
        return answer[node];
    };
    
    for (let i = 0; i < n; i++) dfs(i);
    return answer;
};
