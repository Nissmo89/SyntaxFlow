#include <vector>
#include <set>
#include <string>
#include <algorithm>

class Solution {
public:
    bool isRectangleCover(std::vector<std::vector<int>>& rectangles) {
        long long area = 0;
        int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
        std::set<std::pair<int, int>> corners;

        for (const auto& r : rectangles) {
            minX = std::min(minX, r[0]); minY = std::min(minY, r[1]);
            maxX = std::max(maxX, r[2]); maxY = std::max(maxY, r[3]);
            area += (long long)(r[2] - r[0]) * (r[3] - r[1]);

            std::vector<std::pair<int, int>> pts = {{r[0], r[1]}, {r[0], r[3]}, {r[2], r[1]}, {r[2], r[3]}};
            for (auto& p : pts) {
                if (!corners.insert(p).second) corners.erase(p);
            }
        }

        if (corners.size() != 4 || 
            corners.find({minX, minY}) == corners.end() || 
            corners.find({minX, maxY}) == corners.end() || 
            corners.find({maxX, minY}) == corners.end() || 
            corners.find({maxX, maxY}) == corners.end()) return false;

        return area == (long long)(maxX - minX) * (maxY - minY);
    }
};
