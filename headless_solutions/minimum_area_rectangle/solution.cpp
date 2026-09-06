#include <vector>
#include <unordered_set>
#include <algorithm>
#include <climits>

class Solution {
public:
    int minAreaRect(std::vector<std::vector<int>>& points) {
        std::unordered_set<long long> pointSet;
        for (const auto& p : points) {
            pointSet.insert((long long)p[0] * 40001 + p[1]);
        }
        int minArea = INT_MAX;
        for (int i = 0; i < points.size(); ++i) {
            for (int j = i + 1; j < points.size(); ++j) {
                if (points[i][0] != points[j][0] && points[i][1] != points[j][1]) {
                    if (pointSet.count((long long)points[i][0] * 40001 + points[j][1]) &&
                        pointSet.count((long long)points[j][0] * 40001 + points[i][1])) {
                        minArea = std::min(minArea, std::abs(points[i][0] - points[j][0]) * std::abs(points[i][1] - points[j][1]));
                    }
                }
            }
        }
        return minArea == INT_MAX ? 0 : minArea;
    }
};
