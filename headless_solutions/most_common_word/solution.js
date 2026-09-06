/**
 * @param {string} paragraph
 * @param {string[]} banned
 * @return {string}
 */
var mostCommonWord = function(paragraph, banned) {
    let ban = new Set(banned);
    let words = paragraph.toLowerCase().match(/\b\w+\b/g);
    let count = {};
    for (let w of words) {
        if (!ban.has(w)) {
            count[w] = (count[w] || 0) + 1;
        }
    }
    let max = 0;
    let result = '';
    for (let w in count) {
        if (count[w] > max) {
            max = count[w];
            result = w;
        }
    }
    return result;
};
