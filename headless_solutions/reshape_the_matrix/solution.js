/**
 * @param {number[][]} mat
 * @param {number} r
 * @param {number} c
 * @return {number[][]}
 */
var matrixReshape = function(mat, r, c) {
    let m = mat.length, n = mat[0].length;
    if (m * n != r * c) return mat;
    let res = Array(r).fill(0).map(() => Array(c));
    for (let i = 0; i < m * n; i++) {
        res[i / c][i % c] = mat[i / n][i % n];
    }
    return res;
};
