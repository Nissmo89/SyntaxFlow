/**
 * @param {string} s
 * @return {number}
 */
var findSubstringInWraproundString = function(s) {
    let count = new Array(26).fill(0);
    let maxLengthCur = 0;
    for (let i = 0; i < s.length; i++) {
        if (i > 0 && (s.charCodeAt(i) - s.charCodeAt(i - 1) === 1 || (s[i - 1] === 'z' && s[i] === 'a'))) {
            maxLengthCur++;
        } else {
            maxLengthCur = 1;
        }
        let idx = s.charCodeAt(i) - 97;
        count[idx] = Math.max(count[idx], maxLengthCur);
    }
    return count.reduce((a, b) => a + b, 0);
};
