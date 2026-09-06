/**
 * @param {string} s
 * @param {number} k
 * @return {string}
 */
var decodeAtIndex = function(s, k) {
    let size = 0;
    for (let char of s) {
        if (/\d/.test(char)) size *= parseInt(char);
        else size++;
    }
    for (let i = s.length - 1; i >= 0; i--) {
        k %= size;
        if (k === 0 && /[a-z]/.test(s[i])) return s[i];
        if (/\d/.test(s[i])) size /= parseInt(s[i]);
        else size--;
    }
};
