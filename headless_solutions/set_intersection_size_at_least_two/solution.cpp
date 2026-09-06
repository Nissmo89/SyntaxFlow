#include <vector>
#include <algorithm>

class Solution {
public:
    int intersectionSizeTwo(std::vector<std::vector<int>>& intervals) {
        std::sort(intervals.begin(), intervals.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
            return a[1] < b[1] || (a[1] == b[1] && a[0] > b[0]);
        });
        int p1 = -1, p2 = -1, ans = 0;
        for (const auto& interval : intervals) {
            if (interval[0] > p2) {
                ans += 2;
                p1 = interval[1] - 1;
                p2 = interval[1];
            } else if (interval[0] > p1) {
                ans += 1;
                p1 = p2;
                p2 = interval[1];
            }
        }
        return ans;
    }
};
