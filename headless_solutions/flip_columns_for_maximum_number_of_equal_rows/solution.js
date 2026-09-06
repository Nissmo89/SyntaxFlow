/**
 * @param {number[][]} matrix
 * @return {number}
 */
var maxEqualRowsAfterFlips = function(matrix) {
    const counts = new Map();
    for (const row of matrix) {
        const first = row[0];
        const pattern = row.map(val => val ^ first).join('');
        counts.set(pattern, (counts.get(pattern) || 0) + 1);
    }
    return Math.max(...counts.values());
};
