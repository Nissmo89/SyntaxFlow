/**
 * @param {string[]} words
 * @return {string[]}
 */
var findAllConcatenatedWordsInADict = function(words) {
    // Sort by length
    words.sort((a, b) => a.length() - b.length());

    const dict = new Set();
    const ans = [];

    for (const w of words) {
        const n = w.length();
        const dp = new Array(n + 1).fill(false);
        dp[0] = true;

        for (let i = 1; i <= n; i++) {
            for (let j = 0; j < i; j++) {
                if (!dp[j]) continue;
                const sub = w.slice(j, i);
                if (dict.has(sub)) {
                    dp[i] = true;
                    break;
                }
            }
        }

        if (dp[n]) ans.push(w);
        dict.add(w);
    }
    return ans;
};
