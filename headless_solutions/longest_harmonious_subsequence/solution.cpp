class Solution {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> count;
        for (int num : nums) {
            count[num]++;
        }
        int result = 0;
        for (auto& pair : count) {
            if (count.find(pair.first + 1) != count.end()) {
                result = max(result, pair.second + count[pair.first + 1]);
            }
        }
        return result;
    }
};
