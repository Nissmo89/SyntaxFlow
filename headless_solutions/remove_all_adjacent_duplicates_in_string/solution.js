/**
 * @param {string} s
 * @return {string}
 */
var removeDuplicates = function(s) {
    let res = "";
    for (let c of s) {
        if (res.length > 0 && c === res[res.length - 1]) {
            res = res.slice(0, -1);
        } else {
            res += c;
        }
    }
    return res;
};
