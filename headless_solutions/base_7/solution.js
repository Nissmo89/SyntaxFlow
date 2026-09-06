/**
 * @param {number} num
 * @return {string}
 */
var convertToBase7 = function(num) {
    if (num === 0) {
        return "0";
    }

    let isNegative = num < 0;
    // Work with absolute value for conversion
    let absNum = Math.abs(num);
    
    let digits = []; // Use an array to store digits

    while (absNum > 0) {
        let remainder = absNum % 7;
        digits.push(remainder); // Append the digit (as number, will be converted to string by join)
        absNum = Math.floor(absNum / 7); // Integer division
    }

    // The digits are collected in reverse order, so reverse the array
    digits.reverse(); 

    // Join the array of digits into a single string
    let resultStr = digits.join("");

    if (isNegative) {
        return "-" + resultStr;
    } else {
        return resultStr;
    }
};
