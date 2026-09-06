class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        long long xor_sum = 0;
        for (int num : nums) xor_sum ^= num;
        
        // Isolate the rightmost set bit
        long long diff = xor_sum & -xor_sum;
        
        int a = 0, b = 0;
        for (int num : nums) {
            if (num & diff) a ^= num;
            else b ^= num;
        }
        return {a, b};
    }
};
