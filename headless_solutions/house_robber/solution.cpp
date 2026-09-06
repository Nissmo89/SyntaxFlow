class Solution {
public:
    int rob(vector<int>& nums) {
        int prev2 = 0; // Max loot up to i-2
        int prev1 = 0; // Max loot up to i-1
        
        for (int num : nums) {
            int current = max(prev1, prev2 + num);
            prev2 = prev1;
            prev1 = current;
        }
        return prev1;
    }
};
