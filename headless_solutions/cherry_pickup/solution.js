/**
 * @param {number[][]} grid
 * @return {number}
 */
var cherryPickup = function(grid) {
    const n = grid.length;
    const INF = -1e9;

    let prev = Array.from({length: n}, () => Array(n).fill(INF));
    let cur  = Array.from({length: n}, () => Array(n).fill(INF));

    prev[0][0] = grid[0][0] === -1 ? INF : grid[0][0];

    for (let t = 1; t <= 2 * n - 2; ++t) {
        for (let i = 0; i < n; i++) cur[i].fill(INF);

        const r1Min = Math.max(0, t - (n - 1));
        const r1Max = Math.min(n - 1, t);
        for (let r1 = r1Min; r1 <= r1Max; ++r1) {
            const c1 = t - r1;
            if (grid[r1][c1] === -1) continue;

            const r2Min = Math.max(0, t - (n - 1));
            const r2Max = Math.min(n - 1, t);
            for (let r2 = r2Min; r2 <= r2Max; ++r2) {
                const c2 = t - r2;
                if (grid[r2][c2] === -1) continue;

                let cherries = grid[r1][c1];
                if (r1 !== r2 || c1 !== c2) cherries += grid[r2][c2];

                let best = INF;
                if (r1 > 0 && r2 > 0) best = Math.max(best, prev[r1-1][r2-1]);
                if (r1 > 0)           best = Math.max(best, prev[r1-1][r2]);
                if (r2 > 0)           best = Math.max(best, prev[r1][r2-1]);
                                   best = Math.max(best, prev[r1][r2]);

                if (best !== INF) cur[r1][r2] = best + cherries;
            }
        }
        [prev, cur] = [cur, prev];
    }

    const ans = prev[n-1][n-1];
    return ans > 0 ? ans : 0;
};
