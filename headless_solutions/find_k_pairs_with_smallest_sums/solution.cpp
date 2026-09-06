#include <vector>
#include <queue>

class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        auto cmp = [&](const vector<int>& a, const vector<int>& b) {
            return a[0] + a[1] > b[0] + b[1];
        };
        priority_queue<vector<int>, vector<vector<int>>, decltype(cmp)> pq(cmp);
        
        for (int i = 0; i < min((int)nums1.size(), k); ++i) {
            pq.push({nums1[i], nums2[0], 0});
        }
        
        vector<vector<int>> res;
        while (k-- > 0 && !pq.empty()) {
            vector<int> top = pq.top(); pq.pop();
            res.push_back({top[0], top[1]});
            int j = top[2];
            if (j + 1 < nums2.size()) {
                pq.push({top[0] - nums2[j] + nums2[j + 1], nums2[j + 1], j + 1});
            }
        }
        return res;
    }
};
