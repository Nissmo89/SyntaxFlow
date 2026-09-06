#include <vector>
#include <queue>

class Solution {
public:
    int shortestPathLength(std::vector<std::vector<int>>& graph) {
        int n = graph.size();
        if (n == 1) return 0;
        int target = (1 << n) - 1;
        std::queue<std::pair<int, int>> q;
        std::vector<std::vector<bool>> visited(1 << n, std::vector<bool>(n, false));

        for (int i = 0; i < n; ++i) {
            q.push({1 << i, i});
            visited[1 << i][i] = true;
        }

        int dist = 0;
        while (!q.empty()) {
            int size = q.size();
            while (size--) {
                auto [mask, u] = q.front(); q.pop();
                if (mask == target) return dist;
                for (int v : graph[u]) {
                    int next_mask = mask | (1 << v);
                    if (!visited[next_mask][v]) {
                        visited[next_mask][v] = true;
                        q.push({next_mask, v});
                    }
                }
            }
            dist++;
        }
        return -1;
    }
};
