class Solution {
public:
    vector<int> shortestAlternatingPaths(int n, vector<vector<int>>& redEdges, vector<vector<int>>& blueEdges) {
        vector<vector<int>> adj[2];
        adj[0].resize(n); adj[1].resize(n);
        for (auto& e : redEdges) adj[0][e[0]].push_back(e[1]);
        for (auto& e : blueEdges) adj[1][e[0]].push_back(e[1]);

        vector<vector<int>> dist(n, vector<int>(2, -1));
        queue<pair<int, int>> q;

        dist[0][0] = dist[0][1] = 0;
        q.push({0, 0}); q.push({0, 1});

        while (!q.empty()) {
            auto [u, c] = q.front(); q.pop();
            int next_c = 1 - c;
            for (int v : adj[next_c][u]) {
                if (dist[v][next_c] == -1) {
                    dist[v][next_c] = dist[u][c] + 1;
                    q.push({v, next_c});
                }
            }
        }
        vector<int> res(n);
        for (int i = 0; i < n; ++i) {
            if (dist[i][0] == -1) res[i] = dist[i][1];
            else if (dist[i][1] == -1) res[i] = dist[i][0];
            else res[i] = min(dist[i][0], dist[i][1]);
        }
        return res;
    }
};
