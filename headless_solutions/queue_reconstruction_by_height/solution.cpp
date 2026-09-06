#include <vector>
#include <algorithm>

class Solution {
public:
    std::vector<std::vector<int>> reconstructQueue(std::vector<std::vector<int>>& people) {
        // Sort people:
        // 1. By height in descending order (-p[0])
        // 2. By k-value in ascending order (p[1])
        std::sort(people.begin(), people.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
            if (a[0] != b[0]) {
                return a[0] > b[0]; // Sort by height descending
            }
            return a[1] < b[1]; // Sort by k ascending for same height
        });

        std::vector<std::vector<int>> queue;

        // Insert each person into the queue at their k-th position
        for (const auto& person : people) {
            // person[1] is the k-value, which is the index for insertion
            // std::vector::insert shifts elements to the right
            queue.insert(queue.begin() + person[1], person);
        }

        return queue;
    }
};
