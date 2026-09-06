// JavaScript implementation matching signature
/**
 * @param {number[]} bits
 * @return {boolean}
 */
var isOneBitCharacter = function(bits) {
    let i = 0;
    while (i < bits.length - 1) {
        if (bits[i] === 0) {
            i++;          // 1‑bit character
        } else {
            i += 2;       // 2‑bit character
        }
    }
    return i === bits.length - 1;
};
