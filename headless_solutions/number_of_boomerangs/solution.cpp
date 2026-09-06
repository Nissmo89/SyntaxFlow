#include <vector>
#include <unordered_map>

class Solution {
public:
    int numberOfBoomerangs(std::vector<std::vector<int>>& points) {
        int count = 0;
        for (const auto& p1 : points) {
            std::unordered_map<int, int> distMap;
            for (const auto& p2 : points) {
                int dx = p1[0] - p2[0];
                int dy = p1[1] - p2[1];
                int distSq = dx * dx + dy * dy;
                distMap[distSq]++;
            }
            for (auto const& [dist, freq] : distMap) {
                if (freq >= 2) {
                    count += freq * (freq - 1);
                }
            }
        }
        return count;
    }
};
