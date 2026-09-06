#include <string>
#include <numeric> // For std::gcd
#include <utility> // For std::pair

// Helper function to compute GCD
long long gcd(long long a, long long b) {
    return std::abs(std::gcd(a, b));
}

// Helper function to parse a rational number string into a simplified fraction (numerator, denominator)
std::pair<long long, long long> parseRational(const std::string& s) {
    long long integerPart = 0;
    long long nonRepeatingPart = 0;
    long long repeatingPart = 0;
    int nonRepeatingLen = 0;
    int repeatingLen = 0;

    size_t dotPos = s.find('.');
    size_t parenOpenPos = s.find('(');
    size_t parenClosePos = s.find(')');

    // Extract Integer Part
    if (dotPos == std::string::npos) {
        integerPart = std::stoll(s);
    } else {
        integerPart = std::stoll(s.substr(0, dotPos));
        if (parenOpenPos == std::string::npos) {
            // No repeating part, only non-repeating
            std::string nonRepeatingStr = s.substr(dotPos + 1);
            if (!nonRepeatingStr.empty()) {
                nonRepeatingPart = std::stoll(nonRepeatingStr);
                nonRepeatingLen = nonRepeatingStr.length();
            }
        } else {
            // Has repeating part
            std::string nonRepeatingStr = s.substr(dotPos + 1, parenOpenPos - dotPos - 1);
            if (!nonRepeatingStr.empty()) {
                nonRepeatingPart = std::stoll(nonRepeatingStr);
                nonRepeatingLen = nonRepeatingStr.length();
            }

            std::string repeatingStr = s.substr(parenOpenPos + 1, parenClosePos - parenOpenPos - 1);
            if (!repeatingStr.empty()) {
                repeatingPart = std::stoll(repeatingStr);
                repeatingLen = repeatingStr.length();
            }
        }
    }

    // Calculate the fraction
    long long numerator = 0;
    long long denominator = 1;

    if (repeatingLen == 0) {
        // Case: Integer or Integer.NonRepeating
        numerator = integerPart * std::pow(10, nonRepeatingLen) + nonRepeatingPart;
        denominator = std::pow(10, nonRepeatingLen);
    } else {
        // Case: Integer.NonRepeating(Repeating)
        // Value = Integer + NonRepeating / 10^L_N + Repeating / (10^L_N * (10^L_R - 1))
        // Combine:
        // Numerator = I * 10^L_N * (10^L_R - 1) + N * (10^L_R - 1) + R
        // Denominator = 10^L_N * (10^L_R - 1)

        long long repDen = std::pow(10, repeatingLen) - 1; // 10^L_R - 1
        long long commonDen = std::pow(10, nonRepeatingLen) * repDen; // 10^L_N * (10^L_R - 1)

        numerator = integerPart * commonDen + nonRepeatingPart * repDen + repeatingPart;
        denominator = commonDen;
    }

    // Simplify the fraction
    if (numerator == 0) {
        return {0, 1};
    }
    long long commonDivisor = gcd(numerator, denominator);
    return {numerator / commonDivisor, denominator / commonDivisor};
}

class Solution {
public:
    bool isRationalEqual(std::string s, std::string t) {
        std::pair<long long, long long> s_frac = parseRational(s);
        std::pair<long long, long long> t_frac = parseRational(t);

        return s_frac.first == t_frac.first && s_frac.second == t_frac.second;
    }
};
