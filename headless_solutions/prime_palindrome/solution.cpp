#include <string>
#include <algorithm>
#include <cmath>

class Solution {
public:
    int primePalindrome(int n) {
        if (n >= 8 && n <= 11) return 11;
        for (int len = 1; len <= 9; ++len) {
            // Even length palindromes > 11 are divisible by 11
            if (len % 2 == 0 && len != 2) continue;
            
            int half = (len + 1) / 2;
            int start = pow(10, half - 1);
            int end = pow(10, half) - 1;
            
            for (int i = start; i <= end; ++i) {
                std::string s = std::to_string(i);
                std::string r = s;
                std::reverse(r.begin(), r.end());
                int val = std::stoi(s + r.substr(len % 2));
                if (val >= n && isPrime(val)) return val;
            }
        }
        return -1;
    }

    bool isPrime(int num) {
        if (num < 2) return false;
        for (int i = 2; i * i <= num; ++i)
            if (num % i == 0) return false;
        return true;
    }
};
