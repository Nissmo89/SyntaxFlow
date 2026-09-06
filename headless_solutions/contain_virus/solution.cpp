class Solution {
public:
    int containVirus(vector<vector<int>>& isInfected) {
        int m = isInfected.size();
        int n = isInfected[0].size();
        int totalWalls = 0;

        // Directions: up, down, left, right
        const int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

        while (true) {
            vector<vector<bool>> visited(m, vector<bool>(n, false));
            struct Region {
                set<pair<int,int>> cells;
                set<pair<int,int>> frontier;
                int perimeter = 0;
            };
            vector<Region> regions;

            // 1) Find all infected regions
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (isInfected[i][j] == 1 && !visited[i][j]) {
                        Region reg;
                        queue<pair<int,int>> q;
                        q.emplace(i,j);
                        visited[i][j] = true;

                        while (!q.empty()) {
                            auto [x,y] = q.front(); q.pop();
                            reg.cells.emplace(x,y);
                            // explore 4 neighbours
                            for (auto &d : dirs) {
                                int nx = x + d[0], ny = y + d[1];
                                if (nx < 0 || nx >= m || ny < 0 || ny >= n) {
                                    ++reg.perimeter;               // boundary wall
                                } else if (isInfected[nx][ny] == 0) {
                                    reg.frontier.emplace(nx,ny);
                                    ++reg.perimeter;               // wall to safe cell
                                } else if (isInfected[nx][ny] == 1 && !visited[nx][ny]) {
                                    visited[nx][ny] = true;
                                    q.emplace(nx,ny);
                                }
                            }
                        }
                        regions.push_back(reg);
                    }
                }
            }

            if (regions.empty()) break;

            // 2) Choose region with largest frontier
            Region best = *max_element(regions.begin(), regions.end(),
                [](const Region &a, const Region &b) { return a.frontier.size() < b.frontier.size(); });
            totalWalls += best.perimeter;

            // 3) Quarantine the chosen region
            for (auto [x,y] : best.cells) isInfected[x][y] = 2;

            // 4) Spread virus from remaining 1's
            vector<vector<int>> nextGrid = isInfected;
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (isInfected[i][j] == 1) {
                        for (auto &d : dirs) {
                            int ni = i + d[0], nj = j + d[1];
                            if (ni >= 0 && ni < m && nj >= 0 && nj < n && isInfected[ni][nj] == 0) {
                                nextGrid[ni][nj] = 1;
                            }
                        }
                    }
                }
            }
            isInfected = nextGrid;
        }
        return totalWalls;
    }
};
