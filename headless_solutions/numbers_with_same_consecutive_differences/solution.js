/**
 * @param {number} n
 * @param {number} k
 * @return {number[]}
 */
var numsSameConsecDiff = function(n, k) {
    let cur = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    for (let i = 1; i < n; i++) {
        let next = [];
        for (let x of cur) {
            let y = x % 10;
            if (y + k <= 9) next.push(x * 10 + y + k);
            if (k > 0 && y - k >= 0) next.push(x * 10 + y - k);
        }
        cur = next;
    }
    return cur;
};
