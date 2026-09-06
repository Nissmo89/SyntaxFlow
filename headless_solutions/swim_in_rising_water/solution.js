var swimInWater = function(grid) {
    const n = grid.length;
    const pq = new MinPriorityQueue({ priority: x => x[0] });
    const visited = Array.from({ length: n }, () => Array(n).fill(false));
    
    pq.enqueue([grid[0][0], 0, 0]);
    visited[0][0] = true;
    let res = 0;
    const dirs = [[0, 1], [0, -1], [1, 0], [-1, 0]];
    
    while (!pq.isEmpty()) {
        const [h, r, c] = pq.dequeue().element;
        res = Math.max(res, h);
        if (r === n - 1 && c === n - 1) return res;
        
        for (const [dr, dc] of dirs) {
            const nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && !visited[nr][nc]) {
                visited[nr][nc] = true;
                pq.enqueue([grid[nr][nc], nr, nc]);
            }
        }
    }
};
