/**
 * @param {number} n
 * @return {boolean}
 */
var hasAlternatingBits = function(n) {
    // Step 1: XOR n with n shifted right by 1.
    // If n has alternating bits (e.g., 10101), then n ^ (n >> 1) will result in a number
    // where all bits are 1s (e.g., 11111).
    // JavaScript bitwise operations treat numbers as 32-bit signed integers.
    let temp = n ^ (n >> 1);
    
    // Step 2: Check if 'temp' consists of all 1s.
    // A number 'x' consists of all 1s if and only if (x & (x + 1)) == 0.
    // This is because if x is all 1s (e.g., 0111), then x + 1 will be a power of 2 (e.g., 1000).
    // The bitwise AND of an all-ones number and the next power of 2 will always be 0.
    return (temp & (temp + 1)) === 0;
};
