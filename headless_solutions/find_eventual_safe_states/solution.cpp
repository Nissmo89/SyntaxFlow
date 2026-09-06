class Solution {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<vector<int>> revGraph(n);
        vector<int> outDegree(n, 0);
        queue<int> q;
        
        for (int i = 0; i < n; ++i) {
            outDegree[i] = graph[i].size();
            if (outDegree[i] == 0) q.push(i);
            for (int neighbor : graph[i]) {
                revGraph[neighbor].push_back(i);
            }
        }
        
        vector<int> safeNodes;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            safeNodes.push_back(u);
            for (int v : revGraph[u]) {
                if (--outDegree[v] == 0) q.push(v);
            }
        }
        sort(safeNodes.begin(), safeNodes.end());
        return safeNodes;
    }
};
