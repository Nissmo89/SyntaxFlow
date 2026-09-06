var networkDelayTime = function(times, n, k) {
    const adj = new Map();
    for (const [u, v, w] of times) {
        if (!adj.has(u)) adj.set(u, []);
        adj.get(u).push([v, w]);
    }
    
    const dist = new Array(n + 1).fill(Infinity);
    dist[k] = 0;
    const pq = [[0, k]]; // [distance, node]
    
    while (pq.length > 0) {
        pq.sort((a, b) => a[0] - b[0]);
        const [d, u] = pq.shift();
        
        if (d > dist[u]) continue;
        if (!adj.has(u)) continue;
        
        for (const [v, w] of adj.get(u)) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push([dist[v], v]);
            }
        }
    }
    
    const res = Math.max(...dist.slice(1));
    return res === Infinity ? -1 : res;
};
