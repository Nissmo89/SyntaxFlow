#include <vector>
#include <algorithm>
#include <unordered_map>

class Solution {
public:
    int largestValsFromLabels(std::vector<int>& values, std::vector<int>& labels, int numWanted, int useLimit) {
        int n = values.size();
        std::vector<std::pair<int, int>> items(n);
        for (int i = 0; i < n; ++i) items[i] = {values[i], labels[i]};
        
        // Sort by value descending
        std::sort(items.rbegin(), items.rend());
        
        std::unordered_map<int, int> labelCounts;
        int totalSum = 0, count = 0;
        
        for (auto& item : items) {
            if (count == numWanted) break;
            if (labelCounts[item.second] < useLimit) {
                totalSum += item.first;
                labelCounts[item.second]++;
                count++;
            }
        }
        return totalSum;
    }
};
