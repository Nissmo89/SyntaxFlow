/**
 * @param {number[]} p1
 * @param {number[]} p2
 * @param {number[]} p3
 * @param {number[]} p4
 * @return {boolean}
 */
var validSquare = function(p1, p2, p3, p4) {
    const distSq = (a, b) => (a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2;
    const distances = new Set([
        distSq(p1, p2), distSq(p1, p3), distSq(p1, p4),
        distSq(p2, p3), distSq(p2, p4), distSq(p3, p4)
    ]);
    return distances.size === 2 && !distances.has(0);
};
