/**
 * @param {string} n
 * @return {string}
 */
var smallestGoodBase = function(n) {
    const val = BigInt(n);
    for (let m = 60n; m >= 1n; m--) {
        let low = 2n, high = BigInt(Math.floor(Math.pow(Number(val), 1 / Number(m)))) + 1n;
        while (low <= high) {
            let k = (low + high) / 2n;
            let sum = 0n, p = 1n;
            for (let i = 0n; i <= m; i++) {
                sum += p;
                if (i < m) p *= k;
            }
            if (sum === val) return k.toString();
            if (sum < val) low = k + 1n;
            else high = k - 1n;
        }
    }
    return (val - 1n).toString();
};
