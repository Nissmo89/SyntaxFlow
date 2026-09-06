class Solution {
public:
    int maxCount(int m, int n, vector<vector<int>>& ops) {
        int row = INT_MAX, col = INT_MAX;
        for (auto& op : ops) {
            row = min(row, op[0]);
            col = min(col, op[1]);
        }
        return row * col;
    }
};
