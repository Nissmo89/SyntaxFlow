/**
 * @param {string} s
 * @param {string} p
 * @return {number[]}
 */
var findAnagrams = function(s, p) {
    const n = s.length, m = p.length;
    const ans = [];
    if (m > n) return ans;

    const need = new Array(26).fill(0);
    for (const ch of p) need[ch.charCodeAt(0) - 97]++;

    const window = new Array(26).fill(0);
    for (let i = 0; i < m; ++i) {
        window[s.charCodeAt(i) - 97]++;
    }

    const same = () => {
        for (let i = 0; i < 26; ++i)
            if (need[i] !== window[i]) return false;
        return true;
    };

    if (same()) ans.push(0);

    for (let i = m; i < n; ++i) {
        // remove leftmost char
        window[s.charCodeAt(i - m) - 97]--;
        // add new char
        window[s.charCodeAt(i) - 97]++;

        if (same()) ans.push(i - m + 1);
    }
    return ans;
};
