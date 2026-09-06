/**
 * @param {string} s
 * @param {number[]} shifts
 * @return {string}
 */
var shiftingLetters = function(s, shifts) {
    let chars = s.split('');
    let currentShift = 0;
    for (let i = chars.length - 1; i >= 0; i--) {
        currentShift = (currentShift + shifts[i]) % 26;
        let val = chars[i].charCodeAt(0) - 97;
        chars[i] = String.fromCharCode(((val + currentShift) % 26) + 97);
    }
    return chars.join('');
};
