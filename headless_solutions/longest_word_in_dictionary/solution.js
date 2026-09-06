/**
 * @param {string[]} words
 * @return {string}
 */
var longestWord = function(words) {
    words.sort();
    let built = new Set();
    let res = "";
    for (let w of words) {
        if (w.length === 1 || built.has(w.slice(0, -1))) {
            if (w.length > res.length) res = w;
            built.add(w);
        }
    }
    return res;
};
