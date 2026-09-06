/**
 * @param {number[]} citations
 * @return {number}
 */
var hIndex = function(citations) {
    const n = citations.length;
    const buckets = new Array(n + 1).fill(0);
    for (let c of citations) {
        buckets[Math.min(c, n)]++;
    }
    let count = 0;
    for (let i = n; i >= 0; i--) {
        count += buckets[i];
        if (count >= i) return i;
    }
    return 0;
};
