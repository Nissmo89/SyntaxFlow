/**
 * @param {string} s
 * @return {string[]}
 */
var findRepeatedDnaSequences = function(s) {
    const seen = new Set();
    const repeated = new Set();
    for (let i = 0; i <= s.length - 10; i++) {
        const sub = s.substring(i, i + 10);
        if (seen.has(sub)) {
            repeated.add(sub);
        } else {
            seen.add(sub);
        }
    }
    return Array.from(repeated);
};
