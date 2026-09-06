#include <string>
#include <algorithm>
#include <vector>

class Solution {
public:
    int nextGreaterElement(int n) {
        std::string s = std::to_string(n);
        int len = s.length();
        int i = len - 1;
        while (i > 0 && s[i - 1] >= s[i]) i--;
        if (i == 0) return -1;
        
        int j = len - 1;
        while (s[j] <= s[i - 1]) j--;
        std::swap(s[i - 1], s[j]);
        std::reverse(s.begin() + i, s.end());
        
        long res = std::stol(s);
        return (res > 2147483647) ? -1 : (int)res;
    }
};
