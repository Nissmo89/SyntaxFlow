#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Solution {
public:
    vector<string> addOperators(string num, int target) {
        vector<string> results;
        if (num.empty()) {
            return results;
        }
        // Start the backtracking process
        // Parameters:
        // results: vector to store valid expressions
        // currentExpression: the expression string built so far
        // num: the input digit string
        // target: the target value
        // index: current position in num
        // currentValue: the evaluated value of currentExpression so far
        // lastOperand: the value of the last operand added/subtracted (for multiplication precedence)
        backtrack(results, "", num, (long long)target, 0, 0, 0);
        return results;
    }

private:
    void backtrack(vector<string>& results, string currentExpression, const string& num, long long target, int index, long long currentValue, long long lastOperand) {
        // Base case: If we have processed all digits
        if (index == num.length()) {
            // If the evaluated value matches the target, add the expression to results
            if (currentValue == target) {
                results.push_back(currentExpression);
            }
            return;
        }

        // Iterate through all possible lengths of the next operand
        for (int i = index; i < num.length(); ++i) {
            // Handle leading zeros: if the current digit is '0' and it's not the only digit in the operand, skip.
            // Example: "105", if index is 1 (pointing to '0'), and i is 2 (forming "05"), this is invalid.
            if (i > index && num[index] == '0') {
                break; // Cannot form multi-digit numbers starting with '0'
            }

            // Extract the current operand as a long long
            string currentOperandStr = num.substr(index, i - index + 1);
            long long currentOperand = stoll(currentOperandStr);

            // If this is the first operand (index == 0)
            if (index == 0) {
                // Start the recursion with the first operand
                backtrack(results, currentOperandStr, num, target, i + 1, currentOperand, currentOperand);
            } else {
                // Try adding '+' operator
                backtrack(results, currentExpression + "+" + currentOperandStr, num, target, i + 1, currentValue + currentOperand, currentOperand);

                // Try adding '-' operator
                // Note: lastOperand is stored as negative for subtraction
                backtrack(results, currentExpression + "-" + currentOperandStr, num, target, i + 1, currentValue - currentOperand, -currentOperand);

                // Try adding '*' operator
                // The logic for multiplication:
                // currentValue - lastOperand: undo the effect of the last operand
                // + lastOperand * currentOperand: add the effect of the multiplication
                // The new lastOperand for future multiplications becomes lastOperand * currentOperand
                backtrack(results, currentExpression + "*" + currentOperandStr, num, target, i + 1, currentValue - lastOperand + lastOperand * currentOperand, lastOperand * currentOperand);
            }
        }
    }
};
