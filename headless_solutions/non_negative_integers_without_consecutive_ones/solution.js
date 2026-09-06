/**
 * @param {number} n
 * @return {number}
 */
var findIntegers = function(n) {
    const f = new Array(31).fill(0);
    f[0] = 1;
    f[1] = 2;
    for (let i = 2; i < 31; ++i) {
        f[i] = f[i - 1] + f[i - 2];
    }

    let ans = 0;
    let prevBit = 0;

    for (let k = 30; k >= 0; --k) {
        if ((n & (1 << k)) !== 0) {
            ans += f[k];
            if (prevBit === 1) {
                return ans;
            }
            prevBit = 1;
        } else {
            prevBit = 0;
        }
    }

    return ans + 1;
};
