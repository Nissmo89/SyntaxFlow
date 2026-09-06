/**
 * @param {number[][]} grid
 * @return {number}
 */
var surfaceArea = function(grid) {
    let res = 0, n = grid.length;
    for (let i = 0; i < n; ++i) {
        for (let j = 0; j < n; ++j) {
            if (grid[i][j]) res += 2 + grid[i][j] * 4;
            if (i) res -= Math.min(grid[i][j], grid[i - 1][j]) * 2;
            if (j) res -= Math.min(grid[i][j], grid[i][j - 1]) * 2;
        }
    }
    return res;
};
