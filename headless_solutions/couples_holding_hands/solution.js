var minSwapsCouples = function(row) {
    const n = row.length / 2;
    const parent = Array.from({length: n}, (_, i) => i);
    let count = n;
    const find = (i) => parent[i] === i ? i : (parent[i] = find(parent[i]));
    
    for (let i = 0; i < row.length; i += 2) {
        const u = find(Math.floor(row[i] / 2));
        const v = find(Math.floor(row[i+1] / 2));
        if (u !== v) {
            parent[u] = v;
            count--;
        }
    }
    return n - count;
};
