var characterReplacement = function(s, k) {
    let count = new Array(26).fill(0);
    let left = 0, maxFreq = 0, maxLen = 0;
    for (let right = 0; right < s.length; right++) {
        let charIdx = s.charCodeAt(right) - 65;
        count[charIdx]++;
        maxFreq = Math.max(maxFreq, count[charIdx]);
        
        if ((right - left + 1) - maxFreq > k) {
            count[s.charCodeAt(left) - 65]--;
            left++;
        }
        maxLen = Math.max(maxLen, right - left + 1);
    }
    return maxLen;
};
