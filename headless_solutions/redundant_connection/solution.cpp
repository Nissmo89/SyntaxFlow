class Solution {
    vector<int> parent;
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]); // Path compression
    }
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        int n = edges.size();
        parent.resize(n + 1);
        for (int i = 1; i <= n; ++i) parent[i] = i;
        
        for (auto& edge : edges) {
            int rootU = find(edge[0]);
            int rootV = find(edge[1]);
            if (rootU == rootV) return edge;
            parent[rootU] = rootV;
        }
        return {};
    }
};
