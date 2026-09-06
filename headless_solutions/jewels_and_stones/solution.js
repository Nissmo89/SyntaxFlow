/**
 * @param {string} jewels
 * @param {string} stones
 * @return {number}
 */
var numJewelsInStones = function(jewels, stones) {
    let setJ = new Set(jewels);
    return stones.split('').filter(s => setJ.has(s)).length;
};
