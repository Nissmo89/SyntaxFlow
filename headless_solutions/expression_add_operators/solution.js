/**
 * @param {string} num
 * @param {number} target
 * @return {string[]}
 */
var addOperators = function(num, target) {
    const results = [];
    if (!num || num.length === 0) {
        return results;
    }

    /**
     * Recursive helper function for backtracking.
     * @param {number} index - The current starting index in the 'num' string.
     * @param {string} currentExpression - The expression string built so far.
     * @param {number} currentValue - The evaluated value of the currentExpression.
     * @param {number} lastOperand - The value of the last operand added/subtracted.
     *                               Crucial for handling multiplication precedence.
     */
    function backtrack(index, currentExpression, currentValue, lastOperand) {
        // Base case: If we have processed all digits
        if (index === num.length) {
            // If the evaluated value matches the target, add the expression to results
            if (currentValue === target) {
                results.push(currentExpression);
            }
            return;
        }

        // Iterate through all possible lengths of the next operand
        for (let i = index; i < num.length; i++) {
            // Handle leading zeros: if the current digit is '0' and it's not the only digit in the operand, skip.
            // Example: "105", if index is 1 (pointing to '0'), and i is 2 (forming "05"), this is invalid.
            if (i > index && num[index] === '0') {
                break; // Cannot form multi-digit numbers starting with '0'
            }

            // Extract the current operand as a number
            const currentOperandStr = num.substring(index, i + 1);
            const currentOperand = parseInt(currentOperandStr, 10);

            // If this is the first operand (index == 0)
            if (index === 0) {
                // Start the recursion with the first operand
                backtrack(i + 1, currentOperandStr, currentOperand, currentOperand);
            } else {
                // Try adding '+' operator
                backtrack(i + 1, currentExpression + "+" + currentOperandStr, currentValue + currentOperand, currentOperand);

                // Try adding '-' operator
                // Note: lastOperand is stored as negative for subtraction
                backtrack(i + 1, currentExpression + "-" + currentOperandStr, currentValue - currentOperand, -currentOperand);

                // Try adding '*' operator
                // The logic for multiplication:
                // currentValue - lastOperand: undo the effect of the last operand
                // + lastOperand * currentOperand: add the effect of the multiplication
                // The new lastOperand for future multiplications becomes lastOperand * currentOperand
                backtrack(i + 1, currentExpression + "*" + currentOperandStr, currentValue - lastOperand + lastOperand * currentOperand, lastOperand * currentOperand);
            }
        }
    }

    // Start the backtracking process
    // Initial call: index=0, current_expression="", current_value=0, last_operand=0
    // The first operand is handled specially within the loop.
    backtrack(0, "", 0, 0);
    return results;
};
