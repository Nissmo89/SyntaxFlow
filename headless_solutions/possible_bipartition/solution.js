var possibleBipartition = function(n, dislikes) {
    const adj = Array.from({ length: n + 1 }, () => []);
    for (const [u, v] of dislikes) {
        adj[u].push(v);
        adj[v].push(u);
    }
    const color = new Int8Array(n + 1);
    for (let i = 1; i <= n; i++) {
        if (color[i] === 0) {
            const stack = [i];
            color[i] = 1;
            while (stack.length > 0) {
                const curr = stack.pop();
                for (const neighbor of adj[curr]) {
                    if (color[neighbor] === 0) {
                        color[neighbor] = -color[curr];
                        stack.push(neighbor);
                    } else if (color[neighbor] === color[curr]) return false;
                }
            }
        }
    }
    return true;
};
