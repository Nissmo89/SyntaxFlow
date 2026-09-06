/**
 * @param {number[]} nums
 * @return {boolean[]}
 */
var prefixesDivBy5 = function(nums) {
    const answer = []; // Initialize the result array
    let currentRemainder = 0; // Stores the remainder of the current prefix number when divided by 5

    // Iterate through each digit in the binary array
    for (const num of nums) {
        // Update the currentRemainder using the modular arithmetic recurrence:
        // (previous_remainder * 2 + current_bit) % 5
        // Multiplying by 2 shifts the binary number left by one position.
        // Adding 'num' appends the new bit.
        // Taking modulo 5 keeps the remainder small and prevents overflow.
        currentRemainder = (currentRemainder * 2 + num) % 5;

        // If the remainder is 0, the number is divisible by 5.
        answer.push(currentRemainder === 0);
    }

    return answer; // Return the array of boolean results
};
