var shortestPathLength = function(graph) {
    const n = graph.length;
    if (n === 1) return 0;
    const target = (1 << n) - 1;
    const queue = [];
    const visited = Array.from({ length: 1 << n }, () => new Uint8Array(n));

    for (let i = 0; i < n; i++) {
        queue.push([1 << i, i, 0]);
        visited[1 << i][i] = 1;
    }

    let head = 0;
    while (head < queue.length) {
        const [mask, u, dist] = queue[head++];
        for (const v of graph[u]) {
            const nextMask = mask | (1 << v);
            if (nextMask === target) return dist + 1;
            if (!visited[nextMask][v]) {
                visited[nextMask][v] = 1;
                queue.push([nextMask, v, dist + 1]);
            }
        }
    }
};
