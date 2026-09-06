#include <string>
#include <algorithm>
#include <vector>

class Solution {
public:
    bool reorderedPowerOf2(int n) {
        std::string s = std::to_string(n);
        std::sort(s.begin(), s.end());
        
        for (int i = 0; i < 31; ++i) {
            std::string p = std::to_string(1 << i);
            std::sort(p.begin(), p.end());
            if (s == p) return true;
        }
        return false;
    }
};
