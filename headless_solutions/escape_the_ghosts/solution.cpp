#include <vector>
#include <cmath>
#include <cstdlib>

class Solution {
public:
    bool escapeGhosts(std::vector<std::vector<int>>& ghosts, std::vector<int>& target) {
        int playerDist = std::abs(target[0]) + std::abs(target[1]);
        for (const auto& ghost : ghosts) {
            int ghostDist = std::abs(ghost[0] - target[0]) + std::abs(ghost[1] - target[1]);
            if (ghostDist <= playerDist) return false;
        }
        return true;
    }
};
