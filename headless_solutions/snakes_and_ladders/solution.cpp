class Solution {
public:
    int snakesAndLadders(vector<vector<int>>& board) {
        int n = board.size();
        auto getPos = [&](int s) {
            int r = (s - 1) / n;
            int c = (s - 1) % n;
            if (r % 2 != 0) c = n - 1 - c;
            return make_pair(n - 1 - r, c);
        };
        
        queue<pair<int, int>> q;
        q.push({1, 0});
        vector<bool> visited(n * n + 1, false);
        visited[1] = true;
        
        while (!q.empty()) {
            auto [curr, dist] = q.front(); q.pop();
            for (int i = 1; i <= 6; ++i) {
                int next = curr + i;
                if (next > n * n) break;
                auto [r, c] = getPos(next);
                if (board[r][c] != -1) next = board[r][c];
                if (next == n * n) return dist + 1;
                if (!visited[next]) {
                    visited[next] = true;
                    q.push({next, dist + 1});
                }
            }
        }
        return -1;
    }
};
