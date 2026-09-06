/**
 * @param {number[]} score
 * @return {string[]}
 */
var findRelativeRanks = function(score) {
    let N = score.length;
    let scoreToIndex = {};
    for (let i = 0; i < N; i++) {
        scoreToIndex[score[i]] = i;
    }
    score.sort((a, b) => b - a);
    let answer = new Array(N).fill("");
    for (let i = 0; i < N; i++) {
        let index = scoreToIndex[score[i]];
        if (i == 0) {
            answer[index] = "Gold Medal";
        } else if (i == 1) {
            answer[index] = "Silver Medal";
        } else if (i == 2) {
            answer[index] = "Bronze Medal";
        } else {
            answer[index] = String(i + 1);
        }
    }
    return answer;
};
