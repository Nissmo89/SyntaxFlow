class Solution {
public:
    int totalHammingDistance(vector<int>& nums) {
        int total = 0;
        int n = nums.size();
        for (int i = 0; i < 32; ++i) {
            int count = 0;
            for (int num : nums) {
                if ((num >> i) & 1) count++;
            }
            total += count * (n - count);
        }
        return total;
    }
};
