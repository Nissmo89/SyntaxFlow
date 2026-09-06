/**
 * @param {number} n
 * @param {number} k
 * @return {number[]}
 */
var constructArray = function(n, k) {
    let res = [];
    let left = 1;
    let right = n;
    for (let i = 0; i < n; i++) {
        if (k % 2 == 0) {
            res.push(i < k ? left++ : right--);
        } else {
            res.push(i < k ? right-- : left++);
        }
        k--;
    }
    return res;
};
