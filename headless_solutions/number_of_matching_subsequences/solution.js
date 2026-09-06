var numMatchingSubseq = function(s, words) {
    const buckets = Array.from({ length: 26 }, () => []);
    const ptrs = new Int32Array(words.length);
    
    for (let i = 0; i < words.length; i++) {
        buckets[words[i].charCodeAt(0) - 97].push(i);
    }
    
    let count = 0;
    for (const char of s) {
        const charIdx = char.charCodeAt(0) - 97;
        const current = buckets[charIdx];
        buckets[charIdx] = [];
        for (const idx of current) {
            ptrs[idx]++;
            if (ptrs[idx] === words[idx].length) count++;
            else buckets[words[idx].charCodeAt(ptrs[idx]) - 97].push(idx);
        }
    }
    return count;
};
