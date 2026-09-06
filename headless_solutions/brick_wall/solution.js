var leastBricks = function(wall) {
    let edgeCounts = new Map();
    let maxCount = 0;
    for (let row of wall) {
        let currentSum = 0;
        for (let i = 0; i < row.length - 1; i++) {
            currentSum += row[i];
            if (edgeCounts.has(currentSum)) {
                edgeCounts.set(currentSum, edgeCounts.get(currentSum) + 1);
            } else {
                edgeCounts.set(currentSum, 1);
            }
            if (edgeCounts.get(currentSum) > maxCount) {
                maxCount = edgeCounts.get(currentSum);
            }
        }
    }
    return wall.length - maxCount;
};
