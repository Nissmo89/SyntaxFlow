/**
 * @param {string} a
 * @param {string} b
 * @return {number}
 */
var findLUSlength = function(a, b) {
    // If a and b are equal, return -1
    if (a === b) {
        return -1;
    }
    // Otherwise, return the maximum length of a and b
    return Math.max(a.length, b.length);
};
