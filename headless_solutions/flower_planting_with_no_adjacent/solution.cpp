class Solution {
public:
    vector<int> gardenNoAdj(int n, vector<vector<int>>& paths) {
        vector<vector<int>> adj(n);
        for (const auto& path : paths) {
            adj[path[0] - 1].push_back(path[1] - 1);
            adj[path[1] - 1].push_back(path[0] - 1);
        }
        vector<int> answer(n, 0);
        for (int i = 0; i < n; ++i) {
            bool used[5] = {false};
            for (int neighbor : adj[i]) {
                if (answer[neighbor] != 0) used[answer[neighbor]] = true;
            }
            for (int color = 1; color <= 4; ++color) {
                if (!used[color]) {
                    answer[i] = color;
                    break;
                }
            }
        }
        return answer;
    }
};
