/**
 * @param {string} s
 * @return {string}
 */
var reverseWords = function(s) {
    let strIndex = 0;
    while (strIndex < s.length) {
        // Find the start and end index of the word
        let start = strIndex;
        while (strIndex < s.length && s[strIndex] != ' ') {
            strIndex++;
        }
        // Reverse the word
        let word = s.substring(start, strIndex);
        let reversedWord = word.split('').reverse().join('');
        s = s.substring(0, start) + reversedWord + s.substring(strIndex);
        strIndex++;
    }
    return s;
};
