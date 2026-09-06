class Solution {
public:
    int findMinArrowShots(vector<vector<int>>& points) {
        if (points.empty()) return 0;
        // Sort by end coordinate to greedily pick the rightmost point
        sort(points.begin(), points.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });
        
        int arrows = 1;
        int arrowPos = points[0][1];
        
        for (size_t i = 1; i < points.size(); ++i) {
            // If current balloon starts after the last arrow, we need a new one
            if (points[i][0] > arrowPos) {
                arrows++;
                arrowPos = points[i][1];
            }
        }
        return arrows;
    }
};
