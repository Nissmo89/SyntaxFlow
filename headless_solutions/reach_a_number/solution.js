/**
 * @param {number} target
 * @return {number}
 */
var reachNumber = function(target) {
    target = Math.abs(target);
    let step = 0, sum = 0;
    while (sum < target || (sum - target) % 2 !== 0) {
        step++;
        sum += step;
    }
    return step;
};
