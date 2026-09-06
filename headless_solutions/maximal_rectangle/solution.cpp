class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty()) return 0;
        int rows = matrix.size(), cols = matrix[0].size();
        vector<int> heights(cols + 1, 0);
        int maxArea = 0;
        for (const auto& row : matrix) {
            stack<int> s;
            for (int i = 0; i <= cols; ++i) {
                if (i < cols) heights[i] = (row[i] == '1') ? heights[i] + 1 : 0;
                while (!s.empty() && heights[s.top()] >= heights[i]) {
                    int h = heights[s.top()]; s.pop();
                    int w = s.empty() ? i : i - s.top() - 1;
                    maxArea = max(maxArea, h * w);
                }
                s.push(i);
            }
        }
        return maxArea;
    }
};
