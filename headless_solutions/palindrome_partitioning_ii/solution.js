var minCut = function(s) {
    const n = s.length;
    const cuts = Array.from({length: n}, (_, i) => i);
    for (let i = 0; i < n; i++) {
        // Odd
        for (let l = i, r = i; l >= 0 && r < n && s[l] === s[r]; l--, r++)
            cuts[r] = Math.min(cuts[r], l === 0 ? 0 : cuts[l - 1] + 1);
        // Even
        for (let l = i, r = i + 1; l >= 0 && r < n && s[l] === s[r]; l--, r++)
            cuts[r] = Math.min(cuts[r], l === 0 ? 0 : cuts[l - 1] + 1);
    }
    return cuts[n - 1];
};
