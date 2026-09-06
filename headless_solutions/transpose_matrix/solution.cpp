class Solution {
public:
    vector<vector<int>> transpose(vector<vector<int>>& matrix) {
        int M = matrix.size();
        int N = matrix[0].size();
        vector<vector<int>> ans(N, vector<int>(M, 0));
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < M; i++) {
                ans[j][i] = matrix[i][j];
            }
        }
        return ans;
    }
};
