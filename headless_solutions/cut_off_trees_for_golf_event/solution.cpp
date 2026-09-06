class Solution {
   public:
       int cutOffTree(vector<vector<int>>& forest) {
           int m = forest.size(), n = forest[0].size();
           vector<tuple<int, int, int>> trees;
           for (int i = 0; i < m; ++i) {
               for (int j = 0; j < n; ++j) {
                   if (forest[i][j] > 1) {
                       trees.emplace_back(forest[i][j], i, j);
                   }
               }
           }
           sort(trees.begin(), trees.end());

           int sr = 0, sc = 0, total_steps = 0;
           int dr[] = {-1, 1, 0, 0};
           int dc[] = {0, 0, -1, 1};

           for (auto& [h, tr, tc] : trees) {
               int d = bfs(forest, sr, sc, tr, tc, m, n, dr, dc);
               if (d == -1) return -1;
               total_steps += d;
               sr = tr;
               sc = tc;
           }
           return total_steps;
       }

   private:
       int bfs(const vector<vector<int>>& forest, int sr, int sc, int tr, int tc,
               int m, int n, const int dr[], const int dc[]) {
           if (sr == tr && sc == tc) return 0;
           vector<vector<bool>> visited(m, vector<bool>(n, false));
           queue<tuple<int, int, int>> q;
           q.emplace(sr, sc, 0);
           visited[sr][sc] = true;

           while (!q.empty()) {
               auto [r, c, dist] = q.front(); q.pop();
               for (int i = 0; i < 4; ++i) {
                   int nr = r + dr[i], nc = c + dc[i];
                   if (nr >= 0 && nr < m && nc >= 0 && nc < n &&
                       !visited[nr][nc] && forest[nr][nc] != 0) {
                       if (nr == tr && nc == tc) return dist + 1;
                       visited[nr][nc] = true;
                       q.emplace(nr, nc, dist + 1);
                   }
               }
           }
           return -1;
       }
   };
