class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        int max_val = 0;
        for (int num : nums) max_val = max(max_val, num);
        
        vector<int> points(max_val + 1, 0);
        for (int num : nums) points[num] += num;
        
        int take = 0, skip = 0;
        for (int val : points) {
            int new_take = skip + val;
            int new_skip = max(skip, take);
            take = new_take;
            skip = new_skip;
        }
        return max(take, skip);
    }
};
