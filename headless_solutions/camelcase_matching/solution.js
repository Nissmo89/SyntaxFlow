/**
 * @param {string[]} queries
 * @param {string} pattern
 * @return {boolean[]}
 */
var camelMatch = function(queries, pattern) {
    const res = [];
    for (const q of queries) {
        let j = 0;
        let ok = true;
        for (const c of q) {
            if (j < pattern.length && c === pattern[j]) {
                j++;
            } else if (c >= 'A' && c <= 'Z') {
                ok = false;
                break;
            }
        }
        res.push(ok && j === pattern.length);
    }
    return res;
};
