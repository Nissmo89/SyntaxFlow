/**
 * @param {string} s
 * @return {number}
 */
var lengthOfLongestSubstring = function(s) {
    let lastSeen = new Map();
    let maxLength = 0, left = 0;
    for (let right = 0; right < s.length; right++) {
        let char = s[right];
        if (lastSeen.has(char) && lastSeen.get(char) >= left) {
            left = lastSeen.get(char) + 1;
        }
        lastSeen.set(char, right);
        maxLength = Math.max(maxLength, right - left + 1);
    }
    return maxLength;
};
