#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>

class Solution {
public:
    int maxPoints(std::vector<std::vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;

        int max_overall = 1;

        for (int i = 0; i < n; ++i) {
            // Unordered map key: bit-packed 64-bit integer combining dy and dx
            std::unordered_map<long long, int> slope_counts;
            int local_max = 0;

            for (int j = i + 1; j < n; ++j) {
                int dy = points[j][1] - points[i][1];
                int dx = points[j][0] - points[i][0];

                int g = std::gcd(std::abs(dy), std::abs(dx));
                dy /= g;
                dx /= g;

                // Normalize sign representation
                if (dx < 0 || (dx == 0 && dy < 0)) {
                    dx = -dx;
                    dy = -dy;
                }

                // Pack two 32-bit integers into a single 64-bit key
                long long key = ((long long)dy << 32) | (unsigned int)dx;
                slope_counts[key]++;
                local_max = std::max(local_max, slope_counts[key]);
            }
            max_overall = std::max(max_overall, local_max + 1);
        }

        return max_overall;
    }
};
