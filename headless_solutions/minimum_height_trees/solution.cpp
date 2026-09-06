class Solution {
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        if (n == 1) return {0};
        vector<vector<int>> adj(n);
        vector<int> degree(n, 0);
        for (auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
            degree[edge[0]]++;
            degree[edge[1]]++;
        }
        vector<int> leaves;
        for (int i = 0; i < n; ++i) if (degree[i] == 1) leaves.push_back(i);
        
        int remainingNodes = n;
        while (remainingNodes > 2) {
            remainingNodes -= leaves.size();
            vector<int> newLeaves;
            for (int leaf : leaves) {
                for (int neighbor : adj[leaf]) {
                    if (--degree[neighbor] == 1) newLeaves.push_back(neighbor);
                }
            }
            leaves = newLeaves;
        }
        return leaves;
    }
};
