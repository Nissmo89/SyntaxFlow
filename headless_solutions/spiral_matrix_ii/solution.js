/**
 * @param {number} n
 * @return {number[][]}
 */
var generateMatrix = function(n) {
    const matrix = Array.from({ length: n }, () => Array(n).fill(0));
    let r = 0, c = 0, dr = 0, dc = 1;
    for (let i = 1; i <= n * n; i++) {
        matrix[r][c] = i;
        let nr = r + dr, nc = c + dc;
        if (nr < 0 || nr >= n || nc < 0 || nc >= n || matrix[nr][nc] !== 0) {
            [dr, dc] = [dc, -dr];
        }
        r += dr;
        c += dc;
    }
    return matrix;
};
