#include <vector>
#include <string>
#include <algorithm>
#include <climits>

class Solution {
public:
    int findMinDifference(std::vector<std::string>& timePoints) {
        std::vector<int> minutes;
        for (const std::string& t : timePoints) {
            int h = std::stoi(t.substr(0, 2));
            int m = std::stoi(t.substr(3, 2));
            minutes.push_back(h * 60 + m);
        }
        std::sort(minutes.begin(), minutes.end());
        
        int minDiff = INT_MAX;
        for (size_t i = 1; i < minutes.size(); ++i) {
            minDiff = std::min(minDiff, minutes[i] - minutes[i - 1]);
        }
        
        // Circular difference
        int wrapAround = 1440 - minutes.back() + minutes.front();
        return std::min(minDiff, wrapAround);
    }
};
