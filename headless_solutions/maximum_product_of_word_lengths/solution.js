/**
 * @param {string[]} words
 * @return {number}
 */
var maxProduct = function(words) {
    const n = words.length;
    const masks = new Array(n).fill(0);
    for (let i = 0; i < n; i++) {
        for (let char of words[i]) {
            masks[i] |= (1 << (char.charCodeAt(0) - 97));
        }
    }
    let maxProd = 0;
    for (let i = 0; i < n; i++) {
        for (let j = i + 1; j < n; j++) {
            if ((masks[i] & masks[j]) === 0) {
                maxProd = Math.max(maxProd, words[i].length * words[j].length);
            }
        }
    }
    return maxProd;
};
