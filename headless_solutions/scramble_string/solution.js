/**
 * @param {string} s1
 * @param {string} s2
 * @return {boolean}
 */
var isScramble = function(s1, s2, memo = new Map()) {
    if (s1 === s2) return true;
    const key = s1 + "#" + s2;
    if (memo.has(key)) return memo.get(key);
    
    if (s1.split('').sort().join('') !== s2.split('').sort().join('')) {
        memo.set(key, false);
        return false;
    }
    
    for (let i = 1; i < s1.length; i++) {
        if ((isScramble(s1.slice(0, i), s2.slice(0, i), memo) && isScramble(s1.slice(i), s2.slice(i), memo)) ||
            (isScramble(s1.slice(0, i), s2.slice(s1.length - i), memo) && isScramble(s1.slice(i), s2.slice(0, s1.length - i), memo))) {
            memo.set(key, true);
            return true;
        }
    }
    memo.set(key, false);
    return false;
};
