/**
 * @param {string} s
 * @return {boolean}
 */
var checkValidString = function(s) {
    let cmin = 0, cmax = 0;
    for (let char of s) {
        if (char === '(') {
            cmax++; cmin++;
        } else if (char === ')') {
            cmax--; cmin = Math.max(cmin - 1, 0);
        } else { // '*'
            cmax++; cmin = Math.max(cmin - 1, 0);
        }
        if (cmax < 0) return false;
    }
    return cmin === 0;
};
