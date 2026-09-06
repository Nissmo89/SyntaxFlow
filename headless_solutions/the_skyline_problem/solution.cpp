#include <vector>
#include <algorithm>
#include <set>

class Solution {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        vector<pair<int, int>> events;
        for (auto& b : buildings) {
            events.push_back({b[0], -b[2]}); // Start: negative height
            events.push_back({b[1], b[2]});  // End: positive height
        }
        sort(events.begin(), events.end());
        
        multiset<int> heights = {0};
        vector<vector<int>> res;
        int prev_max = 0;
        
        for (auto& e : events) {
            if (e.second < 0) heights.insert(-e.second);
            else heights.erase(heights.find(e.second));
            
            int cur_max = *heights.rbegin();
            if (cur_max != prev_max) {
                res.push_back({e.first, cur_max});
                prev_max = cur_max;
            }
        }
        return res;
    }
};
