#include <vector>
#include <set>
#include <algorithm>
#include <climits>

class Solution {
public:
    int maxSumSubmatrix(std::vector<std::vector<int>>& matrix, int k) {
        int m = matrix.size();
        int n = matrix[0].size();
        int ans = INT_MIN;

        // Ensure outer loop runs on the smaller dimension
        bool colIsOuter = (m > n);
        int R = colIsOuter ? n : m; // Outer dimension size
        int C = colIsOuter ? m : n; // Inner dimension size

        for (int l = 0; l < R; ++l) {
            std::vector<int> rowSums(C, 0);
            for (int r = l; r < R; ++r) {
                for (int i = 0; i < C; ++i) {
                    rowSums[i] += colIsOuter ? matrix[i][r] : matrix[r][i];
                }

                // Fast path: Try standard Kadane's algorithm first
                int maxKadane = rowSums[0];
                int curKadane = rowSums[0];
                for (int i = 1; i < C; ++i) {
                    curKadane = std::max(rowSums[i], curKadane + rowSums[i]);
                    maxKadane = std::max(maxKadane, curKadane);
                }

                if (maxKadane <= k) {
                    ans = std::max(ans, maxKadane);
                    if (ans == k) return k;
                    continue; // Skip BST logic if Kadane's result is valid
                }

                // Fallback: BST lower_bound search for max subsegment <= k
                std::set<int> prefSet;
                prefSet.insert(0);
                int pref = 0;

                for (int sum : rowSums) {
                    pref += sum;
                    // Find smallest prefix sum >= pref - k
                    auto it = prefSet.lower_bound(pref - k);
                    if (it != prefSet.end()) {
                        ans = std::max(ans, pref - *it);
                        if (ans == k) return k;
                    }
                    prefSet.insert(pref);
                }
            }
        }
        return ans;
    }
};
