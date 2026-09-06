/**
 * @param {number} x
 * @return {number}
 */
var mySqrt = function(x) {
    if (x == 0 || x == 1) {
        return x;
    }
    let start = 1, end = x;
    while (start <= end) {
        let mid = start + Math.floor((end - start) / 2);
        let square = mid * mid;
        if (square == x) {
            return mid;
        } else if (square < x) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return end;
};
