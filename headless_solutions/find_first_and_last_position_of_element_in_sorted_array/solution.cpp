class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        auto findBound = [&](bool isFirst) {
            int low = 0, high = nums.size() - 1, ans = -1;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                if (nums[mid] == target) {
                    ans = mid;
                    if (isFirst) high = mid - 1;
                    else low = mid + 1;
                } else if (nums[mid] < target) low = mid + 1;
                else high = mid - 1;
            }
            return ans;
        };
        return {findBound(true), findBound(false)};
    }
};
