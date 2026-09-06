/**
 * @param {number[]} hours
 * @return {number}
 */
var longestWPI = function(hours) {
    let res = 0, score = 0;
    const seen = new Map();
    for (let i = 0; i < hours.length; i++) {
        score += (hours[i] > 8 ? 1 : -1);
        if (score > 0) {
            res = i + 1;
        } else {
            if (!seen.has(score)) seen.set(score, i);
            if (seen.has(score - 1)) res = Math.max(res, i - seen.get(score - 1));
        }
    }
    return res;
};
