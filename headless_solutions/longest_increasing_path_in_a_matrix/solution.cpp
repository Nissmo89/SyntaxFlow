#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    int m, n;
    vector<vector<int>> memo;
    // Four orthogonal directions: Right, Down, Left, Up
    const int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    int dfs(const vector<vector<int>>& matrix, int r, int c) {
        // Return cached result if already computed
        if (memo[r][c] != 0) {
            return memo[r][c];
        }

        int maxLen = 1;
        for (const auto& dir : dirs) {
            int nr = r + dir[0];
            int nc = c + dir[1];

            // Explore adjacent cells with strictly greater values
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && matrix[nr][nc] > matrix[r][c]) {
                maxLen = max(maxLen, 1 + dfs(matrix, nr, nc));
            }
        }

        return memo[r][c] = maxLen;
    }

public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        
        m = matrix.size();
        n = matrix[0].size();
        memo.assign(m, vector<int>(n, 0));

        int globalMax = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                globalMax = max(globalMax, dfs(matrix, i, j));
            }
        }

        return globalMax;
    }
};
