/**
 * @param {number} candies
 * @param {number} num_people
 * @return {number[]}
 */
var distributeCandies = function(candies, num_people) {
    let res = new Array(num_people).fill(0);
    let i = 0;
    while (candies > 0) {
        res[i % num_people] += Math.min(candies, i + 1);
        candies -= i + 1;
        i += 1;
    }
    return res;
};
