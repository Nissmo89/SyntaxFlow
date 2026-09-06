/**
 * @param {string[]} words
 * @return {number}
 */
var numSpecialEquivGroups = function(words) {
    const set = new Set();
    for (const w of words) {
        const even = [];
        const odd  = [];
        for (let i = 0; i < w.length; ++i) {
            if (i % 2 === 0) even.push(w[i]);
            else            odd.push(w[i]);
        }
        even.sort();   // default lexicographic sort works for single chars
        odd.sort();
        const sig = even.join('') + '#' + odd.join('');
        set.add(sig);
    }
    return set.size;
};
