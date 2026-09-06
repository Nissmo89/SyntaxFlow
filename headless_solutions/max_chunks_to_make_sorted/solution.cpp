class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int chunks = 0;
        int max_so_far = 0;
        for (int i = 0; i < arr.size(); ++i) {
            max_so_far = max(max_so_far, arr[i]);
            // If the max value seen so far equals the current index,
            // we have found a valid chunk boundary.
            if (max_so_far == i) {
                chunks++;
            }
        }
        return chunks;
    }
};
