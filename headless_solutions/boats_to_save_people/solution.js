/**
 * @param {number[]} people
 * @param {number} limit
 * @return {number}
 */
var numRescueBoats = function(people, limit) {
    // Sort ascending to pair lightest with heaviest
    people.sort((a, b) => a - b);
    
    let left = 0;
    let right = people.length - 1;
    let boats = 0;
    
    while (left <= right) {
        // If sum within limit, lightest boards with heaviest
        if (people[left] + people[right] <= limit) {
            left++;
        }
        // Heaviest always boards
        right--;
        boats++;
    }
    return boats;
};
