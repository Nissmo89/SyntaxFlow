/**
 * @param {number[][]} points
 * @return {number}
 */
var numberOfBoomerangs = function(points) {
    let count = 0;
    for (let i = 0; i < points.length; i++) {
        const map = new Map();
        for (let j = 0; j < points.length; j++) {
            const dx = points[i][0] - points[j][0];
            const dy = points[i][1] - points[j][1];
            const distSq = dx * dx + dy * dy;
            map.set(distSq, (map.get(distSq) || 0) + 1);
        }
        for (let freq of map.values()) {
            count += freq * (freq - 1);
        }
    }
    return count;
};
