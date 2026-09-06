class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        unordered_map<int, int> freq;
        int dup = -1, missing = -1;
        
        for (int num : nums) {
            freq[num]++;
            if (freq[num] == 2) {
                dup = num;
            }
        }
        
        int n = nums.size();
        int expectedSum = n * (n + 1) / 2;
        int actualSum = 0;
        for (int num : nums) {
            actualSum += num;
        }
        
        missing = expectedSum - actualSum + dup;
        
        return {dup, missing};
    }
};
