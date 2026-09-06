/**
 * @param {string} s
 * @return {string}
 */
var removeOuterParentheses = function(s) {
    let res = "";
    let opened = 0;
    for (let c of s) {
        if (c == "(" && opened++ > 0) res += c;
        if (c == ")" && opened-- > 1) res += c;
    }
    return res;
};
