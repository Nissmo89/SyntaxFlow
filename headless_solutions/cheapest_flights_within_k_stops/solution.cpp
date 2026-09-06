class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        const int INF = INT_MAX;
        vector<int> dist(n, INF);
        dist[src] = 0;
        
        for (int i = 0; i <= k; ++i) {
            vector<int> temp = dist; // copy current best
            bool updated = false;
            for (const auto& f : flights) {
                int u = f[0], v = f[1], w = f[2];
                if (dist[u] != INF) {
                    if (dist[u] + w < temp[v]) {
                        temp[v] = dist[u] + w;
                        updated = true;
                    }
                }
            }
            dist = temp;
            if (!updated) break; // early stop if no improvement
        }
        
        return dist[dst] == INF ? -1 : dist[dst];
    }
};
