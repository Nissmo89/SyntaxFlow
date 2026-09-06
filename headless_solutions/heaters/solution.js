/**
 * @param {number[]} houses
 * @param {number[]} heaters
 * @return {number}
 */
var findRadius = function(houses, heaters) {
    heaters.sort((a, b) => a - b);
    let maxRadius = 0;
    for (let house of houses) {
        let low = 0, high = heaters.length - 1;
        let dist = Infinity;
        while (low <= high) {
            let mid = Math.floor((low + high) / 2);
            dist = Math.min(dist, Math.abs(heaters[mid] - house));
            if (heaters[mid] < house) low = mid + 1;
            else high = mid - 1;
        }
        maxRadius = Math.max(maxRadius, dist);
    }
    return maxRadius;
};
