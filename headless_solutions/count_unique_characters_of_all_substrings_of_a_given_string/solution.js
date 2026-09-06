/**
 * @param {string} s
 * @return {number}
 */
var uniqueLetterString = function(s) {
    const index = Array.from({ length: 26 }, () => [-1]);
    for (let i = 0; i < s.length; i++) {
        index[s.charCodeAt(i) - 65].push(i);
    }
    for (let i = 0; i < 26; i++) {
        index[i].push(s.length);
    }
    
    let res = 0;
    for (let i = 0; i < 26; i++) {
        for (let j = 1; j < index[i].length - 1; j++) {
            res += (index[i][j] - index[i][j - 1]) * (index[i][j + 1] - index[i][j]);
        }
    }
    return res;
};
