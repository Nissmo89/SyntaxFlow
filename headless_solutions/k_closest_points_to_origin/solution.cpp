#include <vector>
#include <queue>

class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        // Max-heap to store pairs of {squared_distance, {x, y}}
        priority_queue<pair<int, vector<int>>> pq;
        
        for (const auto& p : points) {
            int dist = p[0] * p[0] + p[1] * p[1];
            pq.push({dist, p});
            if (pq.size() > k) {
                pq.pop();
            }
        }
        
        vector<vector<int>> result;
        while (!pq.empty()) {
            result.push_back(pq.top().second);
            pq.pop();
        }
        return result;
    }
};
