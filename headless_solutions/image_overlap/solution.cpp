class Solution {
public:
    int largestOverlap(vector<vector<int>>& img1, vector<vector<int>>& img2) {
        int n = img1.size();
        vector<pair<int, int>> l1, l2;
        for (int r = 0; r < n; ++r)
            for (int c = 0; c < n; ++c) {
                if (img1[r][c] == 1) l1.push_back({r, c});
                if (img2[r][c] == 1) l2.push_back({r, c});
            }
        
        map<pair<int, int>, int> count;
        int res = 0;
        for (auto& p1 : l1)
            for (auto& p2 : l2) {
                pair<int, int> diff = {p2.first - p1.first, p2.second - p1.second};
                count[diff]++;
                res = max(res, count[diff]);
            }
        return res;
    }
};
