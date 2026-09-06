#include <string>
#include <algorithm>

class Solution {
public:
    std::string baseNeg2(int n) {
        if (n == 0) return "0";
        
        std::string result = "";
        while (n != 0) {
            int remainder = n & 1;
            result.push_back(remainder == 1 ? '1' : '0');
            n = (n - remainder) / -2;
        }
        
        std::reverse(result.begin(), result.end());
        return result;
    }
};
