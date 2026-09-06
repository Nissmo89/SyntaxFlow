var numEnclaves = function(grid) {
    const m = grid.length, n = grid[0].length;
    const dfs = (r, c) => {
        if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] === 0) return;
        grid[r][c] = 0;
        dfs(r + 1, c); dfs(r - 1, c);
        dfs(r, c + 1); dfs(r, c - 1);
    };
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            if ((i === 0 || j === 0 || i === m - 1 || j === n - 1) && grid[i][j] === 1)
                dfs(i, j);
        }
    }
    return grid.flat().reduce((a, b) => a + b, 0);
};
