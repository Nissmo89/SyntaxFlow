/**
 * @param {number[][]} people
 * @return {number[][]}
 */
var reconstructQueue = function(people) {
    // Sort people:
    // 1. By height in descending order (-p[0])
    // 2. By k-value in ascending order (p[1])
    people.sort((a, b) => {
        if (a[0] !== b[0]) {
            return b[0] - a[0]; // Sort by height descending
        } else {
            return a[1] - b[1]; // Sort by k ascending for same height
        }
    });

    const queue = [];

    // Insert each person into the queue at their k-th position
    for (const person of people) {
        // person[1] is the k-value, which is the index for insertion.
        // Array.prototype.splice(start, deleteCount, item1, item2, ...)
        // When deleteCount is 0, it inserts elements at the specified start index.
        queue.splice(person[1], 0, person);
    }

    return queue;
};
