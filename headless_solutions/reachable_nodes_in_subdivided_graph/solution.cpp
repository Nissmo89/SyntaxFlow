#include <vector>
#include <queue>
#include <algorithm>

class Solution {
public:
    int reachableNodes(std::vector<std::vector<int>>& edges, int maxMoves, int n) {
        std::vector<std::vector<std::pair<int, int>>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back({e[1], e[2]});
            adj[e[1]].push_back({e[0], e[2]});
        }
        std::priority_queue<std::pair<int, int>> pq;
        std::vector<int> dist(n, -1);
        pq.push({maxMoves, 0});
        dist[0] = maxMoves;
        int count = 0;
        while (!pq.empty()) {
            auto [moves, u] = pq.top(); pq.pop();
            if (moves < dist[u]) continue;
            count++;
            for (auto& [v, cnt] : adj[u]) {
                int remaining = moves - cnt - 1;
                if (remaining >= 0 && remaining > dist[v]) {
                    dist[v] = remaining;
                    pq.push({remaining, v});
                }
            }
        }
        for (auto& e : edges) {
            int u = e[0], v = e[1], cnt = e[2];
            int a = std::max(0, dist[u]);
            int b = std::max(0, dist[v]);
            count += std::min(cnt, a + b);
        }
        return count;
    }
};
