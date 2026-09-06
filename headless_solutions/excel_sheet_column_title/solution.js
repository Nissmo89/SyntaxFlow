/**
 * @param {number} columnNumber
 * @return {string}
 */
var convertToTitle = function(columnNumber) {
    let ans = "";
    while (columnNumber > 0) {
        columnNumber--;
        ans = String.fromCharCode('A'.charCodeAt(0) + columnNumber % 26) + ans;
        columnNumber = Math.floor(columnNumber / 26);
    }
    return ans;
};
