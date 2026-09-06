var orangesRotting = function(grid) {
    let m = grid.length, n = grid[0].length, fresh = 0, minutes = 0;
    let q = [];
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            if (grid[i][j] === 2) q.push([i, j]);
            else if (grid[i][j] === 1) fresh++;
        }
    }
    if (fresh === 0) return 0;
    let dirs = [[0, 1], [0, -1], [1, 0], [-1, 0]];
    let head = 0;
    while (head < q.length && fresh > 0) {
        minutes++;
        let size = q.length - head;
        for (let i = 0; i < size; i++) {
            let [r, c] = q[head++];
            for (let [dr, dc] of dirs) {
                let nr = r + dr, nc = c + dc;
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] === 1) {
                    grid[nr][nc] = 2;
                    fresh--;
                    q.push([nr, nc]);
                }
            }
        }
    }
    return fresh === 0 ? minutes : -1;
};
