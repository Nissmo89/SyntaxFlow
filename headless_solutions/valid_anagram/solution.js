/**
 * @param {string} s
 * @param {string} t
 * @return {boolean}
 */
var isAnagram = function(s, t) {
    if (s.length !== t.length) return false;
    
    let alphabet = new Array(26).fill(0);
    for (let c of s) alphabet[c.charCodeAt(0) - 'a'.charCodeAt(0)]++;
    for (let c of t) alphabet[c.charCodeAt(0) - 'a'.charCodeAt(0)]--;
    
    for (let count of alphabet) {
        if (count !== 0) return false;
    }
    
    return true;
};
