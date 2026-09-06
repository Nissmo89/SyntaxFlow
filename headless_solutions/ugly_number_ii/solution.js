/**
 * @param {number} n
 * @return {number}
 */
var nthUglyNumber = function(n) {
    let ugly = new Array(n);
    ugly[0] = 1;
    let i2 = 0, i3 = 0, i5 = 0;
    for (let i = 1; i < n; i++) {
        let next = Math.min(ugly[i2] * 2, ugly[i3] * 3, ugly[i5] * 5);
        ugly[i] = next;
        if (next === ugly[i2] * 2) i2++;
        if (next === ugly[i3] * 3) i3++;
        if (next === ugly[i5] * 5) i5++;
    }
    return ugly[n - 1];
};
