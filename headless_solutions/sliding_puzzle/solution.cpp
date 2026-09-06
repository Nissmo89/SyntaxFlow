class Solution {
public:
    int slidingPuzzle(vector<vector<int>>& board) {
        string target = "123450";
        string start = "";
        for (auto& row : board) for (int val : row) start += to_string(val);
        
        vector<vector<int>> adj = {{1, 3}, {0, 2, 4}, {1, 5}, {0, 4}, {1, 3, 5}, {2, 4}};
        queue<string> q;
        q.push(start);
        unordered_set<string> visited = {start};
        int moves = 0;
        
        while (!q.empty()) {
            int size = q.size();
            while (size--) {
                string curr = q.front(); q.pop();
                if (curr == target) return moves;
                int zero = curr.find('0');
                for (int next_idx : adj[zero]) {
                    string next = curr;
                    swap(next[zero], next[next_idx]);
                    if (!visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }
            moves++;
        }
        return -1;
    }
};
