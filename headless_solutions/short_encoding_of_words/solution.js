/**
 * @param {string[]} words
 * @return {number}
 */
var minimumLengthEncoding = function(words) {
    let set = new Set(words);
    for (let w of words) {
        for (let i = 1; i < w.length; i++) {
            set.delete(w.substring(i));
        }
    }
    let res = 0;
    for (let w of set) {
        res += w.length + 1;
    }
    return res;
};
