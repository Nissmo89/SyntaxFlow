/**
 * @param {number[][]} points
 * @return {number}
 */
var minAreaFreeRect = function(points) {
    const map = new Map();
    for (let i = 0; i < points.length; i++) {
        for (let j = i + 1; j < points.length; j++) {
            const midX = (points[i][0] + points[j][0]) / 2;
            const midY = (points[i][1] + points[j][1]) / 2;
            const distSq = (points[i][0] - points[j][0]) ** 2 + (points[i][1] - points[j][1]) ** 2;
            const key = `${midX},${midY},${distSq}`;
            if (!map.has(key)) map.set(key, []);
            map.get(key).push([i, j]);
        }
    }
    let minArea = Infinity;
    for (const pairs of map.values()) {
        for (let i = 0; i < pairs.length; i++) {
            for (let j = i + 1; j < pairs.length; j++) {
                const [p1, p2] = [points[pairs[i][0]], points[pairs[i][1]]];
                const p3 = points[pairs[j][0]];
                const side1 = Math.sqrt((p1[0] - p3[0]) ** 2 + (p1[1] - p3[1]) ** 2);
                const side2 = Math.sqrt((p2[0] - p3[0]) ** 2 + (p2[1] - p3[1]) ** 2);
                minArea = Math.min(minArea, side1 * side2);
            }
        }
    }
    return minArea === Infinity ? 0 : minArea;
};
