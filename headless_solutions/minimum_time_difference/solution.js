/**
 * @param {string[]} timePoints
 * @return {number}
 */
var findMinDifference = function(timePoints) {
    const minutes = timePoints.map(t => {
        const [h, m] = t.split(':').map(Number);
        return h * 60 + m;
    }).sort((a, b) => a - b);
    
    let minDiff = Infinity;
    for (let i = 1; i < minutes.length; i++) {
        minDiff = Math.min(minDiff, minutes[i] - minutes[i - 1]);
    }
    
    return Math.min(minDiff, 1440 - minutes[minutes.length - 1] + minutes[0]);
};
