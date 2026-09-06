/**
 * @param {number} buckets
 * @param {number} minutesToDie
 * @param {number} minutesToTest
 * @return {number}
 */
var poorPigs = function(buckets, minutesToDie, minutesToTest) {
    let rounds = Math.floor(minutesToTest / minutesToDie);
    let base = rounds + 1;
    let pigs = 0;
    let states = 1;
    while (states < buckets) {
        states *= base;
        pigs++;
    }
    return pigs;
};
