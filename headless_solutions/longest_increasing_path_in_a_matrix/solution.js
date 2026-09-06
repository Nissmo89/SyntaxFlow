/**
 * @param {number[][]} matrix
 * @return {number}
 */
var longestIncreasingPath = function(matrix) {
    if (!matrix || matrix.length === 0 || matrix[0].length === 0) {
        return 0;
    }

    const m = matrix.length;
    const n = matrix[0].length;
    const memo = Array.from({ length: m }, () => Array(n).fill(0));
    const dirs = [[0, 1], [1, 0], [0, -1], [-1, 0]];

    function dfs(r, c) {
        if (memo[r][c] !== 0) {
            return memo[r][c];
        }

        let maxLen = 1;
        for (const [dr, dc] of dirs) {
            const nr = r + dr;
            const nc = c + dc;

            if (nr >= 0 && nr < m && nc >= 0 && nc < n && matrix[nr][nc] > matrix[r][c]) {
                maxLen = Math.max(maxLen, 1 + dfs(nr, nc));
            }
        }

        memo[r][c] = maxLen;
        return maxLen;
    }

    let globalMax = 0;
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            globalMax = Math.max(globalMax, dfs(i, j));
        }
    }

    return globalMax;
};
