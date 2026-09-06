class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        vector<vector<int>> adj(n);
        for (auto& r : richer) adj[r[1]].push_back(r[0]);
        vector<int> answer(n, -1);
        for (int i = 0; i < n; ++i) dfs(i, adj, quiet, answer);
        return answer;
    }

    int dfs(int node, vector<vector<int>>& adj, vector<int>& quiet, vector<int>& answer) {
        if (answer[node] != -1) return answer[node];
        answer[node] = node;
        for (int neighbor : adj[node]) {
            int candidate = dfs(neighbor, adj, quiet, answer);
            if (quiet[candidate] < quiet[answer[node]]) {
                answer[node] = candidate;
            }
        }
        return answer[node];
    }
};
