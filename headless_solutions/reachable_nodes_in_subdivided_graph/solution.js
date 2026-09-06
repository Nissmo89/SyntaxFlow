/**
 * @param {number[][]} edges
 * @param {number} maxMoves
 * @param {number} n
 * @return {number}
 */
var reachableNodes = function(edges, maxMoves, n) {
    const adj = Array.from({ length: n }, () => []);
    for (const [u, v, cnt] of edges) {
        adj[u].push([v, cnt]);
        adj[v].push([u, cnt]);
    }
    const dist = new Array(n).fill(-1);
    const pq = new MaxPriorityQueue({ priority: x => x[0] });
    pq.enqueue([maxMoves, 0]);
    dist[0] = maxMoves;
    let count = 0;
    while (!pq.isEmpty()) {
        const [moves, u] = pq.dequeue().element;
        if (moves < dist[u]) continue;
        count++;
        for (const [v, cnt] of adj[u]) {
            const remaining = moves - cnt - 1;
            if (remaining >= 0 && remaining > dist[v]) {
                dist[v] = remaining;
                pq.enqueue([remaining, v]);
            }
        }
    }
    for (const [u, v, cnt] of edges) {
        const a = Math.max(0, dist[u]);
        const b = Math.max(0, dist[v]);
        count += Math.min(cnt, a + b);
    }
    return count;
};
