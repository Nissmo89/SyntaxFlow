/**
 * @param {number} x
 * @param {number} y
 * @param {number} bound
 * @return {number[]}
 */
var powerfulIntegers = function(x, y, bound) {
    const res = new Set();
    for (let i = 1; i < bound; i *= x) {
        for (let j = 1; i + j <= bound; j *= y) {
            res.add(i + j);
            if (y === 1) break;
        }
        if (x === 1) break;
    }
    return Array.from(res);
};
