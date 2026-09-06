/**
 * @param {number[][]} matrix
 * @param {number} target
 * @return {boolean}
 */
var searchMatrix = function(matrix, target) {
    const m = matrix.length, n = matrix[0].length;
    let low = 0, high = m * n - 1;
    
    while (low <= high) {
        let mid = Math.floor(low + (high - low) / 2);
        let val = matrix[Math.floor(mid / n)][mid % n];
        if (val === target) return true;
        if (val < target) low = mid + 1;
        else high = mid - 1;
    }
    return false;
};
