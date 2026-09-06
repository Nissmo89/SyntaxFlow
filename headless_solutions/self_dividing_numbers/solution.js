// JavaScript implementation
/**
 * @param {number} left
 * @param {number} right
 * @return {number[]}
 */
var selfDividingNumbers = function(left, right) {
    let res = [];
    for (let i = left; i <= right; i++) {
        if (isSelfDividing(i)) {
            res.push(i);
        }
    }
    return res;

    function isSelfDividing(n) {
        for (let d = n; d > 0; d = Math.floor(d / 10)) {
            let lastDigit = d % 10;
            if (lastDigit == 0 || n % lastDigit != 0) {
                return false;
            }
        }
        return true;
    }
};
