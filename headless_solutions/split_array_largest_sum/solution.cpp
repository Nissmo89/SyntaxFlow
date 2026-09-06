class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        long long left = 0, right = 0;
        for (int num : nums) {
            left = max(left, (long long)num);
            right += num;
        }
        while (left < right) {
            long long mid = left + (right - left) / 2;
            if (canSplit(nums, k, mid)) right = mid;
            else left = mid + 1;
        }
        return (int)left;
    }
private:
    bool canSplit(const vector<int>& nums, int k, long long target) {
        int count = 1;
        long long currentSum = 0;
        for (int num : nums) {
            currentSum += num;
            if (currentSum > target) {
                currentSum = num;
                count++;
                if (count > k) return false;
            }
        }
        return true;
    }
};
