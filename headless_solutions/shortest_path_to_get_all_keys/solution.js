/**
 * @param {string[]} grid
 * @return {number}
 */
var shortestPathAllKeys = function(grid) {
    const m = grid.length, n = grid[0].length;
    let keys = 0, sr, sc;
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            if (grid[i][j] === '@') [sr, sc] = [i, j];
            else if (grid[i][j] >= 'a' && grid[i][j] <= 'f') keys++;
        }
    }
    const target = (1 << keys) - 1;
    const q = [[sr, sc, 0, 0]];
    const visited = new Set([`0,${sr},${sc}`]);
    const dr = [0, 0, 1, -1], dc = [1, -1, 0, 0];
    while (q.length) {
        const [r, c, mask, d] = q.shift();
        if (mask === target) return d;
        for (let i = 0; i < 4; i++) {
            const nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] !== '#') {
                const char = grid[nr][nc];
                let nmask = mask;
                if (char >= 'a' && char <= 'f') nmask |= (1 << (char.charCodeAt(0) - 97));
                if (char >= 'A' && char <= 'F' && !(mask & (1 << (char.charCodeAt(0) - 65)))) continue;
                if (!visited.has(`${nmask},${nr},${nc}`)) {
                    visited.add(`${nmask},${nr},${nc}`);
                    q.push([nr, nc, nmask, d + 1]);
                }
            }
        }
    }
    return -1;
};
