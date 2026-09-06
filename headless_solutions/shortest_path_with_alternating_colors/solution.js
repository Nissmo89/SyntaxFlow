var shortestAlternatingPaths = function(n, redEdges, blueEdges) {
    const adj = [[...Array(n)].map(() => []), [...Array(n)].map(() => [])];
    for (const [u, v] of redEdges) adj[0][u].push(v);
    for (const [u, v] of blueEdges) adj[1][u].push(v);

    const dist = Array.from({ length: n }, () => [-1, -1]);
    const q = [[0, 0], [0, 1]];
    dist[0][0] = dist[0][1] = 0;

    while (q.length) {
        const [u, c] = q.shift();
        const nextC = 1 - c;
        for (const v of adj[nextC][u]) {
            if (dist[v][nextC] === -1) {
                dist[v][nextC] = dist[u][c] + 1;
                q.push([v, nextC]);
            }
        }
    }
    return dist.map(([d0, d1]) => {
        if (d0 === -1) return d1;
        if (d1 === -1) return d0;
        return Math.min(d0, d1);
    });
};
