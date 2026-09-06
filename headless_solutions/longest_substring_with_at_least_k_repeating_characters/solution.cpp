#include <string>
#include <vector>
#include <algorithm>

class Solution {
public:
    int longestSubstring(std::string s, int k) {
        int n = s.length();
        if (n < k) return 0;
        
        std::vector<int> count(26, 0);
        for (char c : s) count[c - 'a']++;
        
        for (int i = 0; i < n; ++i) {
            if (count[s[i] - 'a'] < k) {
                // Split at the invalid character
                int left = longestSubstring(s.substr(0, i), k);
                int right = longestSubstring(s.substr(i + 1), k);
                return std::max(left, right);
            }
        }
        return n;
    }
};
