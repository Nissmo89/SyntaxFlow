#include <vector>
#include <unordered_set>

class Solution {
public:
    long distSq(std::vector<int>& a, std::vector<int>& b) {
        return (long)(a[0] - b[0]) * (a[0] - b[0]) + (long)(a[1] - b[1]) * (a[1] - b[1]);
    }

    bool validSquare(std::vector<int>& p1, std::vector<int>& p2, std::vector<int>& p3, std::vector<int>& p4) {
        std::unordered_set<long> s = {
            distSq(p1, p2), distSq(p1, p3), distSq(p1, p4),
            distSq(p2, p3), distSq(p2, p4), distSq(p3, p4)
        };
        // Must have exactly 2 unique distances and no zero distance (points must be distinct)
        return s.size() == 2 && s.find(0) == s.end();
    }
};
