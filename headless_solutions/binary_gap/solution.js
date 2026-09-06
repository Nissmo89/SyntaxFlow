/**
 * @param {number} n
 * @return {number}
 */
var binaryGap = function(n) {
    let maxDistance = 0;
    let lastPos = -1;
    let pos = 0;
    while (n > 0) {
        if (n % 2 === 1) {
            if (lastPos !== -1) {
                maxDistance = Math.max(maxDistance, pos - lastPos);
            }
            lastPos = pos;
        }
        n = n >> 1;
        pos++;
    }
    return maxDistance;
};
