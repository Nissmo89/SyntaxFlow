/**
 * @param {string} s
 * @param {number} k
 * @return {number}
 */
var longestSubstring = function(s, k) {
    if (s.length < k) return 0;
    
    const map = {};
    for (let char of s) map[char] = (map[char] || 0) + 1;
    
    for (let i = 0; i < s.length; i++) {
        if (map[s[i]] < k) {
            const left = longestSubstring(s.substring(0, i), k);
            const right = longestSubstring(s.substring(i + 1), k);
            return Math.max(left, right);
        }
    }
    return s.length;
};
