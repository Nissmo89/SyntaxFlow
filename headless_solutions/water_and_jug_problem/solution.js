/**
 * @param {number} x
 * @param {number} y
 * @param {number} target
 * @return {boolean}
 */
var canMeasureWater = function(x, y, target) {
    if (x + y < target) return false;
    
    const gcd = (a, b) => (b === 0 ? a : gcd(b, a % b));
    return target % gcd(x, y) === 0;
};
