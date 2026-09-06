var longestPalindrome = function(s) {
    let start = 0, maxLen = 0;
    const expand = (l, r) => {
        while (l >= 0 && r < s.length && s[l] === s[r]) {
            l--; r++;
        }
        return r - l - 1;
    };
    for (let i = 0; i < s.length; i++) {
        let len = Math.max(expand(i, i), expand(i, i + 1));
        if (len > maxLen) {
            maxLen = len;
            start = i - Math.floor((len - 1) / 2);
        }
    }
    return s.substring(start, start + maxLen);
};
