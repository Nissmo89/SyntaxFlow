#include <vector>
#include <queue>
#include <algorithm>

class Solution {
public:
    int networkDelayTime(std::vector<std::vector<int>>& times, int n, int k) {
        std::vector<std::vector<std::pair<int, int>>> adj(n + 1);
        for (const auto& t : times) adj[t[0]].push_back({t[1], t[2]});
        
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        pq.push({0, k});
        
        std::vector<int> dist(n + 1, 1e9);
        dist[k] = 0;
        
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto& [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        
        int res = 0;
        for (int i = 1; i <= n; ++i) res = std::max(res, dist[i]);
        return res == 1e9 ? -1 : res;
    }
};
