var shortestBridge = function(grid) {
    const n = grid.length;
    const q = [];
    const dirs = [[0,1],[0,-1],[1,0],[-1,0]];
    
    const dfs = (r, c) => {
        if (r < 0 || c < 0 || r >= n || c >= n || grid[r][c] !== 1) return;
        grid[r][c] = 2;
        q.push([r, c]);
        for (let [dr, dc] of dirs) dfs(r + dr, c + dc);
    };

    let found = false;
    for (let i = 0; i < n && !found; i++)
        for (let j = 0; j < n && !found; j++)
            if (grid[i][j] === 1) { dfs(i, j); found = true; }

    let dist = 0;
    while (q.length) {
        let size = q.length;
        while (size--) {
            let [r, c] = q.shift();
            for (let [dr, dc] of dirs) {
                let nr = r + dr, nc = c + dc;
                if (nr >= 0 && nc >= 0 && nr < n && nc < n) {
                    if (grid[nr][nc] === 1) return dist;
                    if (grid[nr][nc] === 0) { grid[nr][nc] = 2; q.push([nr, nc]); }
                }
            }
        }
        dist++;
    }
};
