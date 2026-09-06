/**
 * @param {number} a
 * @param {number} b
 * @return {string}
 */
var strWithout3a3b = function(a, b) {
    let res = [];
    while (a > 0 || b > 0) {
        let n = res.length;
        if (n >= 2 && res[n - 1] === res[n - 2]) {
            if (res[n - 1] === 'a') {
                res.push('b'); b--;
            } else {
                res.push('a'); a--;
            }
        } else if (a >= b) {
            res.push('a'); a--;
        } else {
            res.push('b'); b--;
        }
    }
    return res.join('');
};
