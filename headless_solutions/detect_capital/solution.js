/**
 * @param {string} word
 * @return {boolean}
 */
var detectCapitalUse = function(word) {
    let count = 0;
    for (let c of word) {
        if (c === c.toUpperCase()) {
            count++;
        }
    }
    return count === 0 || count === word.length || (count === 1 && word[0] === word[0].toUpperCase());
};
