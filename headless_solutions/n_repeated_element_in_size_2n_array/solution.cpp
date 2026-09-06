class Solution {
public:
    int repeatedNTimes(vector<int>& nums) {
        unordered_map<int, int> count;
        for (int num : nums) {
            count[num]++;
        }
        for (auto& pair : count) {
            if (pair.second > 1) {
                return pair.first;
            }
        }
        return -1; // This should never happen
    }
};
