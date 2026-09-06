/**
 * @param {number} n
 * @param {number} k
 * @param {number} row
 * @param {number} column
 * @return {number}
 */
var knightProbability = function(n, k, row, column) {
    const moves = [
        [-2, -1], [-1, -2], [1, -2], [2, -1],
        [2, 1], [1, 2], [-1, 2], [-2, 1]
    ];

    let dp = Array.from({length: n}, () => Array(n).fill(0));
    dp[row][column] = 1.0;

    for (let step = 0; step < k; step++) {
        const next = Array.from({length: n}, () => Array(n).fill(0));
        for (let r = 0; r < n; r++) {
            for (let c = 0; c < n; c++) {
                const cur = dp[r][c];
                if (cur === 0) continue;
                const prob = cur / 8.0;
                for (const [dr, dc] of moves) {
                    const nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < n && nc >= 0 && nc < n) {
                        next[nr][nc] += prob;
                    }
                }
            }
        }
        dp = next;
    }

    let ans = 0;
    for (let r = 0; r < n; r++) {
        for (let c = 0; c < n; c++) {
            ans += dp[r][c];
        }
    }
    return ans;
};
