class Solution {
public:
    int minPatches(vector<int>& nums, int n) {
        long long miss = 1; // Use long long to prevent overflow for n = 2^31 - 1
        int patches = 0, i = 0;
        while (miss <= n) {
            if (i < nums.size() && nums[i] <= miss) {
                miss += nums[i++];
            } else {
                miss += miss;
                patches++;
            }
        }
        return patches;
    }
};
