/**
 * @param {number} num
 * @return {boolean}
 */
var isPerfectSquare = function(num) {
    if (num < 2) return true;
    let low = 2, high = Math.floor(num / 2);
    while (low <= high) {
        let mid = Math.floor((low + high) / 2);
        if (mid * mid == num) return true;
        else if (mid * mid < num) low = mid + 1;
        else high = mid - 1;
    }
    return false;
};
