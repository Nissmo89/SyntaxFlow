/**
 * @param {string} s
 * @return {number}
 */
var countSubstrings = function(s) {
    let count = 0;
    const expand = (l, r) => {
        let res = 0;
        while (l >= 0 && r < s.length && s[l] === s[r]) {
            res++;
            l--;
            r++;
        }
        return res;
    };
    
    for (let i = 0; i < s.length; i++) {
        count += expand(i, i);
        count += expand(i, i + 1);
    }
    return count;
};
