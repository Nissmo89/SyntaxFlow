#include <string>
#include <cstdio>

class Solution {
public:
    std::string complexNumberMultiply(std::string num1, std::string num2) {
        int a, b, c, d;
        // Parse "a+bi" and "c+di" directly using sscanf
        sscanf(num1.c_str(), "%d+%di", &a, &b);
        sscanf(num2.c_str(), "%d+%di", &c, &d);
        
        // Calculate real and imaginary parts of the product
        int real_part = a * c - b * d;
        int imag_part = a * d + b * c;
        
        // Format into the output string "real+imaginaryi"
        return std::to_string(real_part) + "+" + std::to_string(imag_part) + "i";
    }
};
