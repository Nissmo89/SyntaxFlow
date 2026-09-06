#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        typedef pair<int, pair<int, int>> Node; // {val, {list_idx, element_idx}}
        priority_queue<Node, vector<Node>, greater<Node>> pq;
        int curMax = INT_MIN;
        
        for (int i = 0; i < nums.size(); ++i) {
            pq.push({nums[i][0], {i, 0}});
            curMax = max(curMax, nums[i][0]);
        }
        
        int start = -1, end = -1, minRange = INT_MAX;
        
        while (!pq.empty()) {
            auto [minVal, pos] = pq.top();
            pq.pop();
            int r = pos.first, c = pos.second;
            
            if (curMax - minVal < minRange) {
                minRange = curMax - minVal;
                start = minVal;
                end = curMax;
            }
            
            if (c + 1 < nums[r].size()) {
                int nextVal = nums[r][c + 1];
                pq.push({nextVal, {r, c + 1}});
                curMax = max(curMax, nextVal);
            } else break;
        }
        return {start, end};
    }
};
