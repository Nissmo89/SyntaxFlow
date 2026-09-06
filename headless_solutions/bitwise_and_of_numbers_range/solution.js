/**
 * @param {number} left
 * @param {number} right
 * @return {number}
 */
var rangeBitwiseAnd = function(left, right) {
    let m = left;
    let n = right;
    let moveFactor = 1;
    while (m != n) {
        m >>= 1;
        n >>= 1;
        moveFactor <<= 1;
    }
    return m * moveFactor;
};
