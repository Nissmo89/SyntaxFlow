/**
 * @param {string[]} digits
 * @param {number} n
 * @return {number}
 */
var atMostNGivenDigitSet = function(digits, n) {
    const s = n.toString();
    const k = s.length, m = digits.length;
    let ans = 0;
    
    for (let i = 1; i < k; i++) ans += Math.pow(m, i);
    
    for (let i = 0; i < k; i++) {
        let prefixMatch = false;
        for (const d of digits) {
            if (d < s[i]) ans += Math.pow(m, k - 1 - i);
            else if (d === s[i]) prefixMatch = true;
        }
        if (!prefixMatch) return ans;
    }
    return ans + 1;
};
