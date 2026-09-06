/**
 * @param {string} s1
 * @param {string} s2
 * @return {string[]}
 */
var uncommonFromSentences = function(s1, s2) {
    let count = {};
    for (let word of (s1 + " " + s2).split(" ")) {
        count[word] = (count[word] || 0) + 1;
    }
    let res = [];
    for (let word in count) {
        if (count[word] == 1) {
            res.push(word);
        }
    }
    return res;
};
