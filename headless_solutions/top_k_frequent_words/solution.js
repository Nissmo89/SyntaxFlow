var topKFrequent = function(words, k) {
    const map = new Map();
    for (const w of words) map.set(w, (map.get(w) || 0) + 1);
    
    const sorted = Array.from(map.keys()).sort((a, b) => {
        if (map.get(a) !== map.get(b)) return map.get(b) - map.get(a);
        return a < b ? -1 : 1;
    });
    
    return sorted.slice(0, k);
};
