/**
 * @param {string[]} words
 * @param {string} order
 * @return {boolean}
 */
var isAlienSorted = function(words, order) {
    let mapping = {};
    for (let i = 0; i < order.length; i++) {
        mapping[order[i]] = i;
    }
    for (let i = 1; i < words.length; i++) {
        if (!isSorted(words[i - 1], words[i], mapping)) {
            return false;
        }
    }
    return true;

    function isSorted(s1, s2, mapping) {
        let n = s1.length, m = s2.length;
        for (let i = 0; i < n && i < m; i++) {
            if (s1[i] != s2[i]) {
                if (mapping[s1[i]] < mapping[s2[i]]) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        return n <= m;
    }
};
