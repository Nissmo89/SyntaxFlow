class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int xor = 0, i = 0;
        for (i = 0; i < nums.size(); i++) {
            xor = xor ^ i ^ nums[i];
        }
        return xor ^ i;
    }
};
