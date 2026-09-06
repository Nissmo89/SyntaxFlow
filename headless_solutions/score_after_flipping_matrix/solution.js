/**
 * @param {number[][]} grid
 * @return {number}
 */
var matrixScore = function(grid) {
    const M = grid.length, N = grid[0].length;
    let res = (1 << (N - 1)) * M;
    
    for (let j = 1; j < N; j++) {
        let count = 0;
        for (let i = 0; i < M; i++) {
            if (grid[i][j] === grid[i][0]) count++;
        }
        res += Math.max(count, M - count) * (1 << (N - 1 - j));
    }
    return res;
};
