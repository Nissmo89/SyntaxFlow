/**
 * @param {string} s
 * @return {number}
 */
var minAddToMakeValid = function(s) {
    let openNeeded = 0;
    let balance = 0;
    for (let char of s) {
        if (char === '(') {
            balance++;
        } else {
            if (balance > 0) balance--;
            else openNeeded++;
        }
    }
    return openNeeded + balance;
};
