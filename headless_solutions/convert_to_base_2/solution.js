/**
 * @param {number} n
 * @return {string}
 */
var baseNeg2 = function(n) {
    if (n === 0) return "0";
    
    let digits = [];
    while (n !== 0) {
        let remainder = n & 1;
        digits.push(remainder);
        n = Math.trunc((n - remainder) / -2);
    }
    
    return digits.reverse().join('');
};
