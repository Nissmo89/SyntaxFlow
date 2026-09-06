var minAreaRect = function(points) {
    const set = new Set();
    for (const [x, y] of points) set.add(`${x},${y}`);
    let minArea = Infinity;
    for (let i = 0; i < points.length; i++) {
        for (let j = i + 1; j < points.length; j++) {
            const [x1, y1] = points[i];
            const [x2, y2] = points[j];
            if (x1 !== x2 && y1 !== y2) {
                if (set.has(`${x1},${y2}`) && set.has(`${x2},${y1}`)) {
                    minArea = Math.min(minArea, Math.abs(x1 - x2) * Math.abs(y1 - y2));
                }
            }
        }
    }
    return minArea === Infinity ? 0 : minArea;
};
