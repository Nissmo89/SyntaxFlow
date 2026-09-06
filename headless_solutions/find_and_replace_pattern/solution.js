/**
 * @param {string[]} words
 * @param {string} pattern
 * @return {string[]}
 */
var findAndReplacePattern = function(words, pattern) {
    const normalize = (s) => {
        const map = new Map();
        let res = [];
        for (let char of s) {
            if (!map.has(char)) map.set(char, map.size);
            res.push(map.get(char));
        }
        return res.join(',');
    };

    const pNorm = normalize(pattern);
    return words.filter(w => normalize(w) === pNorm);
};
