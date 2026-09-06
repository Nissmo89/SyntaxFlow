#include <vector>
#include <algorithm>
#include <map>

class Solution {
public:
    vector<int> findRightInterval(vector<vector<int>>& intervals) {
        int n = intervals.size();
        vector<pair<int, int>> starts(n);
        for (int i = 0; i < n; ++i) {
            starts[i] = {intervals[i][0], i};
        }
        sort(starts.begin(), starts.end());
        
        vector<int> res(n);
        for (int i = 0; i < n; ++i) {
            int target = intervals[i][1];
            auto it = lower_bound(starts.begin(), starts.end(), make_pair(target, -1));
            if (it == starts.end()) res[i] = -1;
            else res[i] = it->second;
        }
        return res;
    }
};
