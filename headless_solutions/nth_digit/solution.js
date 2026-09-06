/**
 * @param {number} n
 * @return {number}
 */
var findNthDigit = function(n) {
    let len = 1, count = 9, start = 1;
    while (n > len * count) {
        n -= len * count;
        len++;
        count *= 10;
        start *= 10;
    }
    let num = start + Math.floor((n - 1) / len);
    return parseInt(num.toString()[(n - 1) % len]);
};
