/**
 * @param {number} k
 * @return {number}
 */
var preimageSizeFZF = function(k) {
    const countTrailingZeroes = (x) => {
        let count = 0;
        while (x >= 5) {
            x = Math.floor(x / 5);
            count += x;
        }
        return count;
    };

    const getFirst = (target) => {
        let low = 0, high = 5 * target + 5;
        while (low < high) {
            let mid = Math.floor((low + high) / 2);
            if (countTrailingZeroes(mid) < target) low = mid + 1;
            else high = mid;
        }
        return low;
    };

    return getFirst(k + 1) - getFirst(k);
};
