/**
 * @param {string} text
 * @param {string} first
 * @param {string} second
 * @return {string[]}
 */
var findOcurrences = function(text, first, second) {
    let ans = [];
    let words = text.split(" ");
    for (let i = 2; i < words.length; ++i) {
        if (first === words[i - 2] && second === words[i - 1])
            ans.push(words[i]);
    }
    return ans;
};
