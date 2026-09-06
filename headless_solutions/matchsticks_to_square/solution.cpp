class Solution {
public:
    bool makesquare(vector<int>& matchsticks) {
        int sum = accumulate(matchsticks.begin(), matchsticks.end(), 0);
        if (sum % 4 != 0) return false;
        int target = sum / 4;
        sort(matchsticks.rbegin(), matchsticks.rend());
        vector<int> sides(4, 0);
        return backtrack(matchsticks, sides, 0, target);
    }

    bool backtrack(vector<int>& nums, vector<int>& sides, int idx, int target) {
        if (idx == nums.size()) return true;
        for (int i = 0; i < 4; ++i) {
            if (sides[i] + nums[idx] <= target) {
                sides[i] += nums[idx];
                if (backtrack(nums, sides, idx + 1, target)) return true;
                sides[i] -= nums[idx];
            }
            if (sides[i] == 0) break; // Symmetry pruning
        }
        return false;
    }
};
