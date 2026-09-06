#include <vector>
#include <algorithm>

class Solution {
public:
    int maxChunksToSorted(std::vector<int>& arr) {
        int n = arr.size();
        std::vector<int> minOfRight(n);
        
        // Build suffix minimum array
        minOfRight[n - 1] = arr[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            minOfRight[i] = std::min(arr[i], minOfRight[i + 1]);
        }

        int maxOfLeft = arr[0];
        int cuts = 0;

        // Check each possible split point
        for (int i = 0; i < n - 1; ++i) {
            maxOfLeft = std::max(maxOfLeft, arr[i]);
            if (maxOfLeft <= minOfRight[i + 1]) {
                cuts++;
            }
        }

        return cuts + 1;
    }
};
