/**
 * @param {number[]} candyType
 * @return {number}
 */
var distributeCandies = function(candyType) {
    let uniqueCandies = new Set(candyType);
    return Math.min(uniqueCandies.size, candyType.length / 2);
};
