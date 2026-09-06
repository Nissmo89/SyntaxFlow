/**
 * @param {character[]} tasks
 * @param {number} n
 * @return {number}
 */
var leastInterval = function(tasks, n) {
    const freq = new Array(26).fill(0);
    let maxFreq = 0;
    for (const t of tasks) {
        const idx = t.charCodeAt(0) - 65;
        freq[idx]++;
        maxFreq = Math.max(maxFreq, freq[idx]);
    }
    
    let countMax = 0;
    for (const f of freq) {
        if (f === maxFreq) countMax++;
    }
    
    return Math.max(tasks.length, (maxFreq - 1) * (n + 1) + countMax);
};
