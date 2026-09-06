class Solution {
public:
    int bestRotation(vector<int>& nums) {
        int n = nums.size();
        vector<int> change(n, 0);
        for (int i = 0; i < n; ++i) {
            // The point is lost when k = (i - nums[i] + 1) % n
            change[(i - nums[i] + 1 + n) % n]--;
        }
        int max_score = -1, best_k = 0, current_score = 0;
        for (int k = 0; k < n; ++k) {
            current_score += change[k] + 1; // +1 because each rotation gains a point for the element moving to index N-1
            if (current_score > max_score) {
                max_score = current_score;
                best_k = k;
            }
        }
        return best_k;
    }
};
