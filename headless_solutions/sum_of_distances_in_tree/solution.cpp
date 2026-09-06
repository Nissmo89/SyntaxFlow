class Solution {
    vector<int> count, res;
    vector<vector<int>> adj;
    int N;

    void dfs1(int u, int p) {
        count[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs1(v, u);
            count[u] += count[v];
            res[0] += count[v];
        }
    }

    void dfs2(int u, int p) {
        for (int v : adj[u]) {
            if (v == p) continue;
            res[v] = res[u] - count[v] + (N - count[v]);
            dfs2(v, u);
        }
    }

public:
    vector<int> sumOfDistancesInTree(int n, vector<vector<int>>& edges) {
        N = n;
        adj.assign(n, vector<int>());
        count.assign(n, 0);
        res.assign(n, 0);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        dfs1(0, -1);
        dfs2(0, -1);
        return res;
    }
};
