/**
 * @param {number[]} barcodes
 * @return {number[]}
 */
var rearrangeBarcodes = function(barcodes) {
    const counts = new Map();
    for (const b of barcodes) counts.set(b, (counts.get(b) || 0) + 1);
    
    const sorted = [...counts.entries()].sort((a, b) => b[1] - a[1]);
    const res = new Array(barcodes.length);
    let pos = 0;
    
    for (const [val, freq] of sorted) {
        for (let i = 0; i < freq; i++) {
            if (pos >= barcodes.length) pos = 1;
            res[pos] = val;
            pos += 2;
        }
    }
    return res;
};
