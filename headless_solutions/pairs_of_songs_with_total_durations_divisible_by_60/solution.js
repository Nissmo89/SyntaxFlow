/**
 * @param {number[]} time
 * @return {number}
 */
var numPairsDivisibleBy60 = function(time) {
    const count = new Array(60).fill(0);
    let totalPairs = 0;
    for (const t of time) {
        const rem = t % 60;
        const target = (60 - rem) % 60;
        totalPairs += count[target];
        count[rem]++;
    }
    return totalPairs;
};
