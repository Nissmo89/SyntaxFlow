/**
 * @param {string} s
 * @param {number} k
 * @return {string}
 */
var orderlyQueue = function(s, k) {
    if (k > 1) {
        return s.split('').sort().join('');
    }
    let res = s;
    for (let i = 1; i < s.length; i++) {
        let rotated = s.substring(i) + s.substring(0, i);
        if (rotated < res) {
            res = rotated;
        }
    }
    return res;
};
