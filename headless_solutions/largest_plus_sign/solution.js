var orderOfLargestPlusSign = function(n, mines) {
    let grid = Array.from({ length: n }, () => new Int32Array(n).fill(n));
    for (let [r, c] of mines) grid[r][c] = 0;
    
    for (let i = 0; i < n; i++) {
        let l = 0, r = 0, u = 0, d = 0;
        for (let j = 0; j < n; j++) {
            l = grid[i][j] === 0 ? 0 : l + 1;
            grid[i][j] = Math.min(grid[i][j], l);
            
            r = grid[i][n - 1 - j] === 0 ? 0 : r + 1;
            grid[i][n - 1 - j] = Math.min(grid[i][n - 1 - j], r);
            
            u = grid[j][i] === 0 ? 0 : u + 1;
            grid[j][i] = Math.min(grid[j][i], u);
            
            d = grid[n - 1 - j][i] === 0 ? 0 : d + 1;
            grid[n - 1 - j][i] = Math.min(grid[n - 1 - j][i], d);
        }
    }
    let res = 0;
    for (let i = 0; i < n; i++) res = Math.max(res, ...grid[i]);
    return res;
};
