#include <vector>
#include <cmath>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    double minAreaFreeRect(vector<vector<int>>& points) {
        int n = points.size();
        if (n < 4) return 0.0;
        
        auto hash_func = [](const vector<double>& v) {
            size_t seed = 0;
            for (double x : v) seed ^= hash<double>{}(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        };
        
        unordered_map<vector<double>, vector<pair<int, int>>, decltype(hash_func)> groups(10, hash_func);
        
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double midX = (points[i][0] + points[j][0]) / 2.0;
                double midY = (points[i][1] + points[j][1]) / 2.0;
                double distSq = pow(points[i][0] - points[j][0], 2) + pow(points[i][1] - points[j][1], 2);
                groups[{midX, midY, distSq}].push_back({i, j});
            }
        }
        
        double minArea = -1.0;
        for (auto const& [key, pairs] : groups) {
            if (pairs.size() < 2) continue;
            for (int i = 0; i < pairs.size(); ++i) {
                for (int j = i + 1; j < pairs.size(); ++j) {
                    int p1 = pairs[i].first, p2 = pairs[i].second;
                    int p3 = pairs[j].first;
                    double side1 = sqrt(pow(points[p1][0] - points[p3][0], 2) + pow(points[p1][1] - points[p3][1], 2));
                    double side2 = sqrt(pow(points[p2][0] - points[p3][0], 2) + pow(points[p2][1] - points[p3][1], 2));
                    double area = side1 * side2;
                    if (minArea < 0 || area < minArea) minArea = area;
                }
            }
        }
        return minArea < 0 ? 0.0 : minArea;
    }
};
