#include <string>
#include <numeric>
#include <sstream>

class Solution {
public:
    string fractionAddition(string expression) {
        int A = 0, B = 1, a, b;
        char slash;
        stringstream ss(expression);
        while (ss >> a >> slash >> b) {
            A = A * b + a * B;
            B *= b;
            int common = std::gcd(std::abs(A), std::abs(B));
            A /= common;
            B /= common;
        }
        return to_string(A) + "/" + to_string(B);
    }
};
