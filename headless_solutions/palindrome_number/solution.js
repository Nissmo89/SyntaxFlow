/**
 * @param {number} x
 * @return {boolean}
 */
var isPalindrome = function(x) {
    if (x < 0) return false;
    let reversed = 0;
    let temp = x;
    while (temp != 0) {
        let digit = temp % 10;
        reversed = reversed * 10 + digit;
        temp = Math.floor(temp / 10);
    }
    return reversed == x;
};
