class Solution {
public:
    int partitionDisjoint(vector<int>& nums) {
        int partition_idx = 0;
        int max_left = nums[0];
        int curr_max = nums[0];
        
        for (int i = 1; i < nums.size(); ++i) {
            curr_max = max(curr_max, nums[i]);
            if (nums[i] < max_left) {
                partition_idx = i;
                max_left = curr_max;
            }
        }
        return partition_idx + 1;
    }
};
