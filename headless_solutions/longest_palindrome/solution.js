/**
 * @param {string} s
 * @return {number}
 */
var longestPalindrome = function(s) {
    let frequencyMap = {};
    for (let c of s) {
        frequencyMap[c] = (frequencyMap[c] || 0) + 1;
    }
    let res = 0;
    let hasOddFrequency = false;
    for (let pair in frequencyMap) {
        if (frequencyMap[pair] % 2 == 0) {
            res += frequencyMap[pair];
        } else {
            res += frequencyMap[pair] - 1;
            hasOddFrequency = true;
        }
    }
    if (hasOddFrequency) {
        return res + 1;
    } else {
        return res;
    }
};
