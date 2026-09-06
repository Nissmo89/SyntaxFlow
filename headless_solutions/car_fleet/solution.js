/**
 * @param {number} target
 * @param {number[]} position
 * @param {number[]} speed
 * @return {number}
 */
var carFleet = function(target, position, speed) {
    const cars = position.map((pos, i) => [pos, speed[i]]);
    cars.sort((a, b) => b[0] - a[0]); // descending order
    let max_time = 0;
    let count = 0;
    for (const [pos, s] of cars) {
        const time = (target - pos) / s;
        if (time > max_time) {
            max_time = time;
            count++;
        }
    }
    return count;
};
