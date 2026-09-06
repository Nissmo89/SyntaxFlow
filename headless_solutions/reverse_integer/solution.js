/**
 * @param {number} x
 * @return {number}
 */
var reverse = function(x) {
    const INT_MAX = 2147483647;
    const INT_MIN = -2147483648;
    let rev = 0;
    
    while (x !== 0) {
        let pop = x % 10;
        x = Math.trunc(x / 10);
        
        if (rev > INT_MAX / 10 || (rev === Math.floor(INT_MAX / 10) && pop > 7)) return 0;
        if (rev < INT_MIN / 10 || (rev === Math.ceil(INT_MIN / 10) && pop < -8)) return 0;
        
        rev = rev * 10 + pop;
    }
    return rev;
};
