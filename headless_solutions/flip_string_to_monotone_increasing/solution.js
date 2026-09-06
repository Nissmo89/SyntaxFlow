/**
 * @param {string} s
 * @return {number}
 */
var minFlipsMonoIncr = function(s) {
    let countOne = 0;
    let countFlip = 0;
    for (let char of s) {
        if (char === '1') {
            countOne++;
        } else {
            countFlip = Math.min(countFlip + 1, countOne);
        }
    }
    return countFlip;
};
