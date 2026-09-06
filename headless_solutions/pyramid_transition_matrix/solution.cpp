#include <vector>
#include <string>
#include <unordered_map>

class Solution {
public:
    bool pyramidTransition(std::string bottom, std::vector<std::string>& allowed) {
        std::unordered_map<std::string, std::vector<char>> m;
        for (const std::string& s : allowed) {
            m[s.substr(0, 2)].push_back(s[2]);
        }
        return solve(bottom, m, 0, "");
    }

private:
    bool solve(std::string bottom, std::unordered_map<std::string, std::vector<char>>& m, int idx, std::string next) {
        if (bottom.length() == 1) return true;
        if (idx == bottom.length() - 1) return solve(next, m, 0, "");
        
        std::string pair = bottom.substr(idx, 2);
        if (m.find(pair) == m.end()) return false;
        
        for (char c : m[pair]) {
            if (solve(bottom, m, idx + 1, next + c)) return true;
        }
        return false;
    }
};
