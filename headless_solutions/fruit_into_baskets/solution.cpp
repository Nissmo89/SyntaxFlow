#include <vector>
#include <unordered_map>
#include <algorithm>

class Solution {
public:
    int totalFruit(std::vector<int>& fruits) {
        std::unordered_map<int, int> counts;
        int left = 0, max_len = 0;
        for (int right = 0; right < fruits.size(); ++right) {
            counts[fruits[right]]++;
            while (counts.size() > 2) {
                counts[fruits[left]]--;
                if (counts[fruits[left]] == 0) counts.erase(fruits[left]);
                left++;
            }
            max_len = std::max(max_len, right - left + 1);
        }
        return max_len;
    }
};
