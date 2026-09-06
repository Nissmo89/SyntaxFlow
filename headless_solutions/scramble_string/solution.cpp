#include <string>
#include <vector>
#include <unordered_map>

class Solution {
    std::unordered_map<std::string, bool> memo;
public:
    bool isScramble(std::string s1, std::string s2) {
        if (s1 == s2) return true;
        if (s1.length() != s2.length()) return false;
        
        std::string key = s1 + "#" + s2;
        if (memo.count(key)) return memo[key];
        
        int n = s1.length();
        std::vector<int> count(26, 0);
        for (int i = 0; i < n; ++i) {
            count[s1[i] - 'a']++;
            count[s2[i] - 'a']--;
        }
        for (int i : count) if (i != 0) return memo[key] = false;
        
        for (int i = 1; i < n; ++i) {
            if ((isScramble(s1.substr(0, i), s2.substr(0, i)) && isScramble(s1.substr(i), s2.substr(i))) ||
                (isScramble(s1.substr(0, i), s2.substr(n - i)) && isScramble(s1.substr(i), s2.substr(0, n - i)))) {
                return memo[key] = true;
            }
        }
        return memo[key] = false;
    }
};
