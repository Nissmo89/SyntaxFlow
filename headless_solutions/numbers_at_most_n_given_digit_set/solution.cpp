#include <vector>
#include <string>
#include <cmath>

class Solution {
public:
    int atMostNGivenDigitSet(std::vector<std::string>& digits, int n) {
        std::string s = std::to_string(n);
        int k = s.size(), m = digits.size(), ans = 0;
        
        // 1. Count numbers with fewer digits
        for (int i = 1; i < k; ++i) ans += std::pow(m, i);
        
        // 2. Count numbers with exactly k digits
        for (int i = 0; i < k; ++i) {
            bool prefix_match = false;
            for (const std::string& d : digits) {
                if (d[0] < s[i]) ans += std::pow(m, k - 1 - i);
                else if (d[0] == s[i]) prefix_match = true;
            }
            if (!prefix_match) return ans;
        }
        return ans + 1;
    }
};
