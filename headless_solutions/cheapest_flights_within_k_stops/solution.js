/**
 * @param {number} n
 * @param {number[][]} flights
 * @param {number} src
 * @param {number} dst
 * @param {number} k
 * @return {number}
 */
var findCheapestPrice = function(n, flights, src, dst, k) {
    const INF = Number.MAX_SAFE_INTEGER;
    const dist = new Array(n).fill(INF);
    dist[src] = 0;
    
    for (let i = 0; i <= k; i++) {
        const temp = dist.slice(); // copy current best
        let updated = false;
        for (const [u, v, w] of flights) {
            if (dist[u] !== INF) {
                if (dist[u] + w < temp[v]) {
                    temp[v] = dist[u] + w;
                    updated = true;
                }
            }
        }
        dist = temp;
        if (!updated) break;
    }
    
    return dist[dst] === INF ? -1 : dist[dst];
};
