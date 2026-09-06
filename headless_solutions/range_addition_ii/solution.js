/**
 * @param {number} m
 * @param {number} n
 * @param {number[][]} ops
 * @return {number}
 */
var maxCount = function(m, n, ops) {
    if (!ops.length) {
        return m * n;
    }
    let row = Infinity, col = Infinity;
    for (let op of ops) {
        row = Math.min(row, op[0]);
        col = Math.min(col, op[1]);
    }
    return row * col;
};
