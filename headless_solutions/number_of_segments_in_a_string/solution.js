/**
 * @param {string} s
 * @return {number}
 */
var countSegments = function(s) {
    let res = 0;
    for (let i = 0; i < s.length; i++) {
        if (s[i] != ' ' && (i == 0 || s[i - 1] == ' ')) {
            res++;
        }
    }
    return res;
};
