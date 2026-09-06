/**
 * @param {string} expression
 * @return {string}
 */
var fractionAddition = function(expression) {
    const nums = expression.match(/[+-]?\d+/g).map(Number);
    let A = 0, B = 1;
    
    const gcd = (a, b) => (b === 0 ? a : gcd(b, a % b));
    
    for (let i = 0; i < nums.length; i += 2) {
        let a = nums[i], b = nums[i + 1];
        A = A * b + a * B;
        B *= b;
        let common = gcd(Math.abs(A), Math.abs(B));
        A /= common;
        B /= common;
    }
    return `${A}/${B}`;
};
