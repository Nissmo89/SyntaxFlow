/**
 * @param {string[]} operations
 * @return {number}
 */
var calPoints = function(operations) {
    let record = []; // Use an array as a stack
    let totalSum = 0;

    for (let op of operations) {
        if (op === "+") {
            // Sum of previous two scores
            // Problem constraints guarantee at least two scores for '+'
            let score1 = record[record.length - 1]; // Last score
            let score2 = record[record.length - 2]; // Second last score
            let newScore = score1 + score2;
            record.push(newScore);
            totalSum += newScore;
        } else if (op === "D") {
            // Double of previous score
            // Problem constraints guarantee at least one score for 'D'
            let lastScore = record[record.length - 1];
            let newScore = 2 * lastScore;
            record.push(newScore);
            totalSum += newScore;
        } else if (op === "C") {
            // Invalidate previous score
            // Problem constraints guarantee at least one score for 'C'
            let removedScore = record.pop(); // Remove and return the last element
            totalSum -= removedScore;
        } else {
            // An integer x
            let score = parseInt(op); // Convert string to integer
            record.push(score);
            totalSum += score;
        }
    }

    return totalSum;
};
