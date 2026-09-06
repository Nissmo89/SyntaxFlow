var uniquePathsIII = function(grid) {
    let empty = 0, sx = 0, sy = 0;
    const m = grid.length, n = grid[0].length;
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            if (grid[i][j] === 1) { sx = i; sy = j; }
            else if (grid[i][j] === 0) empty++;
        }
    }
    let res = 0;
    const dfs = (x, y, count) => {
        if (x < 0 || x >= m || y < 0 || y >= n || grid[x][y] === -1) return;
        if (grid[x][y] === 2) {
            if (count === empty + 1) res++;
            return;
        }
        grid[x][y] = -1;
        dfs(x + 1, y, count + 1);
        dfs(x - 1, y, count + 1);
        dfs(x, y + 1, count + 1);
        dfs(x, y - 1, count + 1);
        grid[x][y] = 0;
    };
    dfs(sx, sy, 0);
    return res;
};
