#include <vector>
#include <deque>
#include <algorithm>

class Solution {
public:
    int shortestSubarray(std::vector<int>& nums, int k) {
        int n = nums.size();
        std::vector<long long> P(n + 1, 0);
        for (int i = 0; i < n; ++i) P[i + 1] = P[i] + nums[i];
        
        int res = n + 1;
        std::deque<int> dq;
        for (int i = 0; i <= n; ++i) {
            while (!dq.empty() && P[i] - P[dq.front()] >= k) {
                res = std::min(res, i - dq.front());
                dq.pop_front();
            }
            while (!dq.empty() && P[i] <= P[dq.back()]) {
                dq.pop_back();
            }
            dq.push_back(i);
        }
        return res <= n ? res : -1;
    }
};
