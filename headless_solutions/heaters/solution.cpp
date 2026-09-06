#include <vector>
#include <algorithm>
#include <cmath>

class Solution {
public:
    int findRadius(std::vector<int>& houses, std::vector<int>& heaters) {
        std::sort(heaters.begin(), heaters.end());
        int max_radius = 0;
        for (int house : houses) {
            auto it = std::lower_bound(heaters.begin(), heaters.end(), house);
            int dist1 = (it == heaters.begin()) ? INT_MAX : house - *(it - 1);
            int dist2 = (it == heaters.end()) ? INT_MAX : *it - house;
            max_radius = std::max(max_radius, std::min(dist1, dist2));
        }
        return max_radius;
    }
};
