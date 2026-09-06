class DSU {
    vector<int> parent, sz;
public:
    DSU(int n) {
        parent.resize(n); iota(parent.begin(), parent.end(), 0);
        sz.assign(n, 1);
    }
    int find(int i) { return parent[i] == i ? i : parent[i] = find(parent[i]); }
    void unite(int i, int j) {
        int root_i = find(i), root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
            sz[root_j] += sz[root_i];
        }
    }
    int size(int i) { return sz[find(i)]; }
};

class Solution {
public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        int R = grid.size(), C = grid[0].size();
        vector<vector<int>> status = grid;
        for (auto& h : hits) if (status[h[0]][h[1]] == 1) status[h[0]][h[1]] = 0;
        
        DSU dsu(R * C + 1);
        auto get_id = [&](int r, int c) { return r * C + c + 1; };
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                if (status[r][c] == 1) {
                    if (r == 0) dsu.unite(get_id(r, c), 0);
                    if (r > 0 && status[r - 1][c] == 1) dsu.unite(get_id(r, c), get_id(r - 1, c));
                    if (c > 0 && status[r][c - 1] == 1) dsu.unite(get_id(r, c), get_id(r, c - 1));
                }
            }
        }
        
        vector<int> res(hits.size());
        int dr[] = {0, 0, 1, -1}, dc[] = {1, -1, 0, 0};
        for (int i = hits.size() - 1; i >= 0; --i) {
            int r = hits[i][0], c = hits[i][1];
            if (grid[r][c] == 0) continue;
            int prev_stable = dsu.size(0);
            status[r][c] = 1;
            for (int k = 0; k < 4; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < R && nc >= 0 && nc < C && status[nr][nc] == 1)
                    dsu.unite(get_id(r, c), get_id(nr, nc));
            }
            if (r == 0) dsu.unite(get_id(r, c), 0);
            res[i] = max(0, dsu.size(0) - prev_stable - 1);
        }
        return res;
    }
};
