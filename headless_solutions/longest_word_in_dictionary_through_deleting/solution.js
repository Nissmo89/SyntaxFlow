/**
 * @param {string} s
 * @param {string[]} dictionary
 * @return {string}
 */
var findLongestWord = function(s, dictionary) {
    let longest = "";
    for (const word of dictionary) {
        let i = 0;
        for (const char of s) {
            if (i < word.length && char === word[i]) i++;
        }
        if (i === word.length) {
            if (word.length > longest.length || (word.length === longest.length && word < longest)) {
                longest = word;
            }
        }
    }
    return longest;
};
