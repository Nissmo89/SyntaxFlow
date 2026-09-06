class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> colors(n, 0);
        for (int i = 0; i < n; ++i) {
            if (colors[i] == 0 && !dfs(graph, colors, i, 1)) return false;
        }
        return true;
    }
private:
    bool dfs(vector<vector<int>>& graph, vector<int>& colors, int node, int color) {
        colors[node] = color;
        for (int neighbor : graph[node]) {
            if (colors[neighbor] == color) return false;
            if (colors[neighbor] == 0 && !dfs(graph, colors, neighbor, -color)) return false;
        }
        return true;
    }
};
