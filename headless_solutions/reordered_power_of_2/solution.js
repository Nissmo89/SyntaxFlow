/**
 * @param {number} n
 * @return {boolean}
 */
var reorderedPowerOf2 = function(n) {
    const sortDigits = (num) => num.toString().split('').sort().join('');
    const target = sortDigits(n);
    
    for (let i = 0; i < 31; i++) {
        if (target === sortDigits(1 << i)) return true;
    }
    return false;
};
