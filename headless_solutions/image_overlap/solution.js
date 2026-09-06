var largestOverlap = function(img1, img2) {
    const n = img1.length;
    const l1 = [], l2 = [];
    for (let r = 0; r < n; r++) {
        for (let c = 0; c < n; c++) {
            if (img1[r][c]) l1.push([r, c]);
            if (img2[r][c]) l2.push([r, c]);
        }
    }
    const count = new Map();
    let res = 0;
    for (const [r1, c1] of l1) {
        for (const [r2, c2] of l2) {
            const diff = `${r2 - r1},${c2 - c1}`;
            const val = (count.get(diff) || 0) + 1;
            count.set(diff, val);
            res = Math.max(res, val);
        }
    }
    return res;
};
