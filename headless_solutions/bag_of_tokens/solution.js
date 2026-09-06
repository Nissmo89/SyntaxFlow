/**
 * @param {number[]} tokens
 * @param {number} power
 * @return {number}
 */
var bagOfTokensScore = function(tokens, power) {
    tokens.sort((a, b) => a - b);
    let score = 0, maxScore = 0, left = 0, right = tokens.length - 1;
    while (left <= right) {
        if (power >= tokens[left]) {
            power -= tokens[left++];
            score++;
            maxScore = Math.max(maxScore, score);
        } else if (score > 0) {
            power += tokens[right--];
            score--;
        } else {
            break;
        }
    }
    return maxScore;
};
