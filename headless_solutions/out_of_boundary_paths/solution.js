var findPaths = function(m, n, maxMove, startRow, startColumn) {
    const MOD = 1e9 + 7;
    let dp = Array.from({ length: m }, () => new Array(n).fill(0));
    dp[startRow][startColumn] = 1;
    let count = 0;
    const dirs = [[-1, 0], [1, 0], [0, -1], [0, 1]];

    for (let move = 0; move < maxMove; move++) {
        let nextDp = Array.from({ length: m }, () => new Array(n).fill(0));
        for (let r = 0; r < m; r++) {
            for (let c = 0; c < n; c++) {
                if (dp[r][c] === 0) continue;
                for (let [dr, dc] of dirs) {
                    let nr = r + dr, nc = c + dc;
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) {
                        count = (count + dp[r][c]) % MOD;
                    } else {
                        nextDp[nr][nc] = (nextDp[nr][nc] + dp[r][c]) % MOD;
                    }
                }
            }
        }
        dp = nextDp;
    }
    return count;
};
