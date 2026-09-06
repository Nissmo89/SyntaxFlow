#include <vector>
#include <algorithm>

class Solution {
public:
    int minSwap(std::vector<int>& nums1, std::vector<int>& nums2) {
        int n = nums1.size();
        int keep = 0; // Min swaps for subproblem 0..i if we DON'T swap at i
        int swap = 1; // Min swaps for subproblem 0..i if we DO swap at i

        for (int i = 1; i < n; ++i) {
            int new_keep = n + 1;
            int new_swap = n + 1;

            // Case 1: Natural relative order holds without crossing
            if (nums1[i - 1] < nums1[i] && nums2[i - 1] < nums2[i]) {
                new_keep = std::min(new_keep, keep);
                new_swap = std::min(new_swap, swap + 1);
            }

            // Case 2: Crossed order holds
            if (nums1[i - 1] < nums2[i] && nums2[i - 1] < nums1[i]) {
                new_keep = std::min(new_keep, swap);
                new_swap = std::min(new_swap, keep + 1);
            }

            keep = new_keep;
            swap = new_swap;
        }

        return std::min(keep, swap);
    }
};
