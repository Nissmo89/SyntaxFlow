/**
 * @param {string} a
 * @param {string} b
 * @return {string}
 */
var addBinary = function(a, b) {
    let result = []; // Use an array to build the result characters
    let i = a.length - 1;
    let j = b.length - 1;
    let carry = 0;

    // Loop while there are digits in either string or a carry exists
    while (i >= 0 || j >= 0 || carry === 1) {
        let sum = carry; // Start sum with the carry from the previous step

        // Add digit from string 'a' if available
        if (i >= 0) {
            sum += parseInt(a[i]); // Convert char '0'/'1' to int 0/1
            i--; // Move to the next digit in 'a'
        }

        // Add digit from string 'b' if available
        if (j >= 0) {
            sum += parseInt(b[j]); // Convert char '0'/'1' to int 0/1
            j--; // Move to the next digit in 'b'
        }

        // Append the current digit (sum % 2) to the result
        result.push(sum % 2); // Push int 0/1

        // Update carry for the next iteration
        carry = Math.floor(sum / 2); // Use Math.floor for integer division
    }

    // The result is built in reverse order, so reverse it and join
    return result.reverse().join('');
};
