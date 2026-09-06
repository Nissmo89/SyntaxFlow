/**
 * @param {number} x
 * @param {number} y
 * @return {number}
 */
var hammingDistance = function(x, y) {
    let result = x ^ y;
    let count = 0;
    while (result) {
        count++;
        result &= result - 1;
    }
    return count;
};
