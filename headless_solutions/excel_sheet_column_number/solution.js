/**
 * @param {string} columnTitle
 * @return {number}
 */
var titleToNumber = function(columnTitle) {
    let result = 0;
    for (let c of columnTitle) {
        result *= 26;
        result += c.charCodeAt(0) - 'A'.charCodeAt(0) + 1;
    }
    return result;
};
