class Solution {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        // Sort by the end time (second element)
        sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });
        
        int count = 0;
        int current_end = INT_MIN;
        
        for (const auto& p : pairs) {
            if (p[0] > current_end) {
                current_end = p[1];
                count++;
            }
        }
        return count;
    }
};
