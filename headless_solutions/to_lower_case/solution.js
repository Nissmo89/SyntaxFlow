/**
 * @param {string} s
 * @return {string}
 */
var toLowerCase = function(s) {
    let result = "";
    for (let c of s) {
        if (c >= 'A' && c <= 'Z') {
            result += String.fromCharCode(c.charCodeAt(0) + 32);
        } else {
            result += c;
        }
    }
    return result;
};
