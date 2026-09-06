#include <string>
#include <algorithm> // For std::reverse

class Solution {
public:
    std::string addBinary(std::string a, std::string b) {
        std::string result = ""; // Use std::string to build the result
        int i = a.length() - 1;
        int j = b.length() - 1;
        int carry = 0;

        // Loop while there are digits in either string or a carry exists
        while (i >= 0 || j >= 0 || carry == 1) {
            int sum = carry; // Start sum with the carry from the previous step

            // Add digit from string 'a' if available
            if (i >= 0) {
                sum += a[i] - '0'; // Convert char '0'/'1' to int 0/1
                i--; // Move to the next digit in 'a'
            }

            // Add digit from string 'b' if available
            if (j >= 0) {
                sum += b[j] - '0'; // Convert char '0'/'1' to int 0/1
                j--; // Move to the next digit in 'b'
            }

            // Append the current digit (sum % 2) to the result
            result.push_back((sum % 2) + '0'); // Convert int 0/1 back to char '0'/'1'

            // Update carry for the next iteration
            carry = sum / 2;
        }

        // The result is built in reverse order, so reverse it
        std::reverse(result.begin(), result.end());

        return result;
    }
};
