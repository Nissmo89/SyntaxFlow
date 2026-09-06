#include <string>
#include <algorithm> // For std::reverse
#include <vector>    // For building char by char

class Solution {
public:
    string addStrings(string num1, string num2) {
        std::string result = ""; // Use std::string and build it by appending chars
        int i = num1.length() - 1;
        int j = num2.length() - 1;
        int carry = 0;

        // Loop until both numbers are processed and there's no carry left
        while (i >= 0 || j >= 0 || carry == 1) {
            int digit1 = 0;
            if (i >= 0) {
                digit1 = num1[i] - '0'; // Convert char to int
            }

            int digit2 = 0;
            if (j >= 0) {
                digit2 = num2[j] - '0'; // Convert char to int
            }

            int current_sum = digit1 + digit2 + carry;
            result.push_back((current_sum % 10) + '0'); // Convert int back to char and append
            carry = current_sum / 10;

            i--; // Move to the next digit in num1
            j--; // Move to the next digit in num2
        }

        // The result is built in reverse order, so reverse it
        std::reverse(result.begin(), result.end());
        return result;
    }
};
