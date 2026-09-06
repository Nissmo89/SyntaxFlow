var spellchecker = function(wordlist, queries) {
    const exact = new Set(wordlist);
    const cap = new Map();
    const vowel = new Map();
    
    const getMask = (w) => w.toLowerCase().replace(/[aeiou]/g, '#');
    
    for (const w of wordlist) {
        const l = w.toLowerCase();
        const m = getMask(w);
        if (!cap.has(l)) cap.set(l, w);
        if (!vowel.has(m)) vowel.set(m, w);
    }
    
    return queries.map(q => {
        if (exact.has(q)) return q;
        const l = q.toLowerCase();
        if (cap.has(l)) return cap.get(l);
        const m = getMask(q);
        if (vowel.has(m)) return vowel.get(m);
        return "";
    });
};
