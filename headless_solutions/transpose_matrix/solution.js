/**
 * @param {number[][]} matrix
 * @return {number[][]}
 */
var transpose = function(matrix) {
    let M = matrix.length;
    let N = matrix[0].length;
    let ans = Array(N).fill(0).map(() => Array(M).fill(0));
    for (let j = 0; j < N; j++) {
        for (let i = 0; i < M; i++) {
            ans[j][i] = matrix[i][j];
        }
    }
    return ans;
};
