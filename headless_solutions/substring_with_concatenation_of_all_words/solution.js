/**
 * @param {string} s
 * @param {string[]} words
 * @return {number[]}
 */
var findSubstring = function(s, words) {
    const res = [];
    if (!s || !words.length) return res;
    const counts = new Map();
    for (const w of words) counts.set(w, (counts.get(w) || 0) + 1);
    const n = s.length, m = words.length, len = words[0].length;
    
    for (let i = 0; i < len; i++) {
        let seen = new Map(), left = i, count = 0;
        for (let j = i; j <= n - len; j += len) {
            let w = s.substring(j, j + len);
            if (counts.has(w)) {
                seen.set(w, (seen.get(w) || 0) + 1);
                count++;
                while (seen.get(w) > counts.get(w)) {
                    let leftW = s.substring(left, left + len);
                    seen.set(leftW, seen.get(leftW) - 1);
                    count--;
                    left += len;
                }
                if (count === m) res.push(left);
            } else {
                seen.clear();
                count = 0;
                left = j + len;
            }
        }
    }
    return res;
};
