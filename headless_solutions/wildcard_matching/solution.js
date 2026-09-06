var isMatch = function(s, p) {
    let sIdx = 0, pIdx = 0, starIdx = -1, sTmpIdx = -1;
    while (sIdx < s.length) {
        if (pIdx < p.length && (p[pIdx] === '?' || p[pIdx] === s[sIdx])) {
            sIdx++; pIdx++;
        } else if (pIdx < p.length && p[pIdx] === '*') {
            starIdx = pIdx++;
            sTmpIdx = sIdx;
        } else if (starIdx !== -1) {
            pIdx = starIdx + 1;
            sIdx = ++sTmpIdx;
        } else return false;
    }
    while (pIdx < p.length && p[pIdx] === '*') pIdx++;
    return pIdx === p.length;
};
