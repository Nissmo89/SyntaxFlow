/**
 * @param {number[]} seats
 * @return {number}
 */
var maxDistToClosest = function(seats) {
    let last = -1, maxDist = 0;
    for (let i = 0; i < seats.length; i++) {
        if (seats[i] === 1) {
            if (last === -1) maxDist = i;
            else maxDist = Math.max(maxDist, Math.floor((i - last) / 2));
            last = i;
        }
    }
    return Math.max(maxDist, seats.length - 1 - last);
};
