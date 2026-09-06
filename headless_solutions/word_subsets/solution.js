/**
 * @param {string[]} words1
 * @param {string[]} words2
 * @return {string[]}
 */
var wordSubsets = function(words1, words2) {
    const bmax = new Array(26).fill(0);
    for (const b of words2) {
        const count = new Array(26).fill(0);
        for (let i = 0; i < b.length; i++) count[b.charCodeAt(i) - 97]++;
        for (let i = 0; i < 26; i++) bmax[i] = Math.max(bmax[i], count[i]);
    }
    
    return words1.filter(a => {
        const count = new Array(26).fill(0);
        for (let i = 0; i < a.length; i++) count[a.charCodeAt(i) - 97]++;
        for (let i = 0; i < 26; i++) {
            if (count[i] < bmax[i]) return false;
        }
        return true;
    });
};
