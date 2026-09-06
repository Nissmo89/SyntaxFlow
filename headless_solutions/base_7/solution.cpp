#include <string>
#include <algorithm> // For std::reverse

class Solution {
public:
    std::string convertToBase7(int num) {
        if (num == 0) {
            return "0";
        }

        bool is_negative = num < 0;
        // Work with absolute value for conversion
        int abs_num = std::abs(num); 
        
        std::string result_digits = ""; // Use string to build digits

        while (abs_num > 0) {
            int remainder = abs_num % 7;
            // Convert remainder to char and append.
            // '0' + remainder converts int 0-6 to char '0'-'6'
            result_digits += (char)('0' + remainder); 
            abs_num /= 7;
        }

        // The digits are collected in reverse order, so reverse the string
        std::reverse(result_digits.begin(), result_digits.end());

        if (is_negative) {
            return "-" + result_digits;
        } else {
            return result_digits;
        }
    }
};
