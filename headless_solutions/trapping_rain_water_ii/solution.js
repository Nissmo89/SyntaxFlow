/**
 * @param {number[][]} heightMap
 * @return {number}
 */
var trapRainWater = function(heightMap) {
    const m = heightMap.length, n = heightMap[0].length;
    const pq = new MinPriorityQueue({ priority: x => x[0] });
    const visited = Array.from({ length: m }, () => Array(n).fill(false));
    
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            if (i === 0 || i === m - 1 || j === 0 || j === n - 1) {
                pq.enqueue([heightMap[i][j], i, j]);
                visited[i][j] = true;
            }
        }
    }
    
    let res = 0;
    const dirs = [[0, 1], [0, -1], [1, 0], [-1, 0]];
    while (!pq.isEmpty()) {
        const [h, r, c] = pq.dequeue().element;
        for (const [dr, dc] of dirs) {
            const nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && !visited[nr][nc]) {
                res += Math.max(0, h - heightMap[nr][nc]);
                pq.enqueue([Math.max(h, heightMap[nr][nc]), nr, nc]);
                visited[nr][nc] = true;
            }
        }
    }
    return res;
};
