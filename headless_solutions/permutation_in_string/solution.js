var checkInclusion = function(s1, s2) {
    const n = s1.length, m = s2.length;
    if (n > m) return false;
    const s1Count = new Array(26).fill(0);
    const s2Count = new Array(26).fill(0);
    const getIdx = (char) => char.charCodeAt(0) - 97;
    for (let i = 0; i < n; i++) {
        s1Count[getIdx(s1[i])]++;
        s2Count[getIdx(s2[i])]++;
    }
    const matches = (a, b) => a.every((val, i) => val === b[i]);
    if (matches(s1Count, s2Count)) return true;
    for (let i = n; i < m; i++) {
        s2Count[getIdx(s2[i])]++;
        s2Count[getIdx(s2[i - n])]--;
        if (matches(s1Count, s2Count)) return true;
    }
    return false;
};
