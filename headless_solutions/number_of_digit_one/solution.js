/**
 * @param {number} n
 * @return {number}
 */
var countDigitOne = function(n) {
    let count = 0;
    for (let m = 1; m <= n; m *= 10) {
        let a = Math.floor(n / (m * 10));
        let b = n % m;
        let x = Math.floor(n / m) % 10;
        
        count += a * m;
        if (x > 1) count += m;
        else if (x === 1) count += (b + 1);
    }
    return count;
};
