class Solution {
public:
    bool xorGame(vector<int>& nums) {
        int xor_sum = 0;
        for (int num : nums) xor_sum ^= num;
        return xor_sum == 0 || nums.size() % 2 == 0;
    }
};
