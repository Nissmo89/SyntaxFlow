/**
 * @param {string[]} words
 * @return {number}
 */
var longestStrChain = function(words) {
    words.sort((a, b) => a.length - b.length);
    const dp = new Map();
    let maxChain = 0;
    for (const word of words) {
        dp.set(word, 1);
        for (let i = 0; i < word.length; i++) {
            const prev = word.slice(0, i) + word.slice(i + 1);
            if (dp.has(prev)) {
                dp.set(word, Math.max(dp.get(word), dp.get(prev) + 1));
            }
        }
        maxChain = Math.max(maxChain, dp.get(word));
    }
    return maxChain;
};
