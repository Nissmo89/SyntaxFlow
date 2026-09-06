class Solution {
public:
    int countTriplets(vector<int>& nums) {
        int count[65536] = {0};
        for (int a : nums) {
            for (int b : nums) {
                count[a & b]++;
            }
        }
        int ans = 0;
        for (int a : nums) {
            for (int i = 0; i < 65536; ++i) {
                if ((a & i) == 0) {
                    ans += count[i];
                }
            }
        }
        return ans;
    }
};
