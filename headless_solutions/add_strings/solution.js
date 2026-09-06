/**
 * @param {string} num1
 * @param {string} num2
 * @return {string}
 */
var addStrings = function(num1, num2) {
    let result = []; // Use an array of characters to build the result
    let i = num1.length - 1;
    let j = num2.length - 1;
    let carry = 0;

    // Loop until both numbers are processed and there's no carry left
    while (i >= 0 || j >= 0 || carry) {
        let digit1 = i >= 0 ? parseInt(num1[i]) : 0;
        let digit2 = j >= 0 ? parseInt(num2[j]) : 0;

        let current_sum = digit1 + digit2 + carry;
        result.push(current_sum % 10); // Append the digit
        carry = Math.floor(current_sum / 10); // Integer division for carry

        i--; // Move to the next digit in num1
        j--; // Move to the next digit in num2
    }
    
    // The result is built in reverse order, so reverse the array and join
    return result.reverse().join('');
};
