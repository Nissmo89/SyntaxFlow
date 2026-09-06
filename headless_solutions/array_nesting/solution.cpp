class Solution {
public:
    int arrayNesting(vector<int>& nums) {
        int n = nums.size();
        vector<bool> visited(n, false);
        int max_length = 0;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int length = 0;
                int current = i;
                while (!visited[current]) {
                    visited[current] = true;
                    length++;
                    current = nums[current];
                }
                max_length = max(max_length, length);
            }
        }
        
        return max_length;
    }
};
