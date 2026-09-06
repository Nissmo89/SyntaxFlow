/**
 * @param {number} num
 * @return {string}
 */
var toHex = function(num) {
    if (num === 0) return "0";
    const map = '0123456789abcdef';
    let result = '';
    if (num < 0) {
        num += 2**32;
    }
    while (num > 0) {
        const n = num & 15;
        const c = map[n];
        result = c + result;
        num >>>= 4;
    }
    return result;
};
