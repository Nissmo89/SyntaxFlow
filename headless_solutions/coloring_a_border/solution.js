/**
 * @param {number[][]} grid
 * @param {number} row
 * @param {number} col
 * @param {number} color
 * @return {number[][]}
 */
var colorBorder = function(grid, row, col, color) {
    if (!grid || grid.length === 0) return grid;
    const m = grid.length, n = grid[0].length;
    const original = grid[row][col];
    // If the colour is already the target, nothing changes.
    if (original === color) return grid;

    const visited = Array.from({ length: m }, () => Array(n).fill(false));
    const component = [];
    const dirs = [[-1,0],[1,0],[0,-1],[0,1]];

    // -------- DFS to collect component --------
    const dfs = (r, c) => {
        if (r < 0 || r >= m || c < 0 || c >= n) return;
        if (visited[r][c] || grid[r][c] !== original) return;
        visited[r][c] = true;
        component.push([r, c]);
        for (const [dr, dc] of dirs) {
            dfs(r + dr, c + dc);
        }
    };
    dfs(row, col);

    // -------- Determine border cells and recolour --------
    for (const [r, c] of component) {
        let isBorder = false;
        for (const [dr, dc] of dirs) {
            const nr = r + dr, nc = c + dc;
            if (nr < 0 || nr >= m || nc < 0 || nc >= n || grid[nr][nc] !== original) {
                isBorder = true;
                break;
            }
        }
        if (isBorder) {
            grid[r][c] = color;
        }
    }
    return grid;
};
