var regionsBySlashes = function(grid) {
    const n = grid.length;
    const g = Array.from({ length: n * 3 }, () => new Array(n * 3).fill(0));
    for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) {
            if (grid[i][j] === '/') {
                g[i * 3][j * 3 + 2] = g[i * 3 + 1][j * 3 + 1] = g[i * 3 + 2][j * 3] = 1;
            } else if (grid[i][j] === '\\') {
                g[i * 3][j * 3] = g[i * 3 + 1][j * 3 + 1] = g[i * 3 + 2][j * 3 + 2] = 1;
            }
        }
    }
    const dfs = (i, j) => {
        if (i < 0 || j < 0 || i >= n * 3 || j >= n * 3 || g[i][j] !== 0) return;
        g[i][j] = 1;
        dfs(i + 1, j); dfs(i - 1, j); dfs(i, j + 1); dfs(i, j - 1);
    };
    let count = 0;
    for (let i = 0; i < n * 3; i++) {
        for (let j = 0; j < n * 3; j++) {
            if (g[i][j] === 0) {
                dfs(i, j);
                count++;
            }
        }
    }
    return count;
};
