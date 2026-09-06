#include <vector>
#include <string>
#include <queue>
#include <tuple>

class Solution {
public:
    int shortestPathAllKeys(std::vector<std::string>& grid) {
        int m = grid.size(), n = grid[0].size(), keys = 0, startR, startC;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == '@') { startR = i; startC = j; }
                else if (grid[i][j] >= 'a' && grid[i][j] <= 'f') keys++;
            }
        }
        int target = (1 << keys) - 1;
        std::queue<std::vector<int>> q;
        q.push({startR, startC, 0, 0}); // r, c, mask, dist
        std::vector<std::vector<std::vector<bool>>> visited(m, std::vector<std::vector<bool>>(n, std::vector<bool>(1 << keys, false)));
        visited[startR][startC][0] = true;
        int dr[] = {0, 0, 1, -1}, dc[] = {1, -1, 0, 0};
        while (!q.empty()) {
            auto curr = q.front(); q.pop();
            int r = curr[0], c = curr[1], mask = curr[2], d = curr[3];
            if (mask == target) return d;
            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i], nc = c + dc[i];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] != '#') {
                    char cell = grid[nr][nc];
                    int nmask = mask;
                    if (cell >= 'a' && cell <= 'f') nmask |= (1 << (cell - 'a'));
                    if (cell >= 'A' && cell <= 'F' && !(mask & (1 << (cell - 'A')))) continue;
                    if (!visited[nr][nc][nmask]) {
                        visited[nr][nc][nmask] = true;
                        q.push({nr, nc, nmask, d + 1});
                    }
                }
            }
        }
        return -1;
    }
};
