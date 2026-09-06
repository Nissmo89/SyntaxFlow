var isRectangleCover = function(rectangles) {
    let area = 0, minX = Infinity, minY = Infinity, maxX = -Infinity, maxY = -Infinity;
    let corners = new Set();

    for (let [x1, y1, x2, y2] of rectangles) {
        minX = Math.min(minX, x1); minY = Math.min(minY, y1);
        maxX = Math.max(maxX, x2); maxY = Math.max(maxY, y2);
        area += (x2 - x1) * (y2 - y1);
        for (let p of [`${x1},${y1}`, `${x1},${y2}`, `${x2},${y1}`, `${x2},${y2}`]) {
            if (corners.has(p)) corners.delete(p);
            else corners.add(p);
        }
    }
    let expected = [`${minX},${minY}`, `${minX},${maxY}`, `${maxX},${minY}`, `${maxX},${maxY}`];
    for (let e of expected) if (!corners.delete(e)) return false;
    return corners.size === 0 && area === (maxX - minX) * (maxY - minY);
};
