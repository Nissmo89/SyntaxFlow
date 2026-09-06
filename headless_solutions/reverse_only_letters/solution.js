/**
 * @param {string} s
 * @return {string}
 */
var reverseOnlyLetters = function(s) {
    let i = 0, j = s.length - 1;
    while (i < j) {
        while (i < j && !s[i].match(/[a-zA-Z]/)) ++i;
        while (i < j && !s[j].match(/[a-zA-Z]/)) --j;
        let arr = s.split('');
        [arr[i], arr[j]] = [arr[j], arr[i]];
        s = arr.join('');
        i += 1;
        j -= 1;
    }
    return s;
};
