#include <vector>
#include <algorithm>

class Solution {
public:
    std::vector<int> numMovesStones(int a, int b, int c) {
        std::vector<int> s = {a, b, c};
        std::sort(s.begin(), s.end());
        
        if (s[2] - s[0] == 2) return {0, 0};
        
        int min_moves = (s[1] - s[0] <= 2 || s[2] - s[1] <= 2) ? 1 : 2;
        int max_moves = s[2] - s[0] - 2;
        
        return {min_moves, max_moves};
    }
};
