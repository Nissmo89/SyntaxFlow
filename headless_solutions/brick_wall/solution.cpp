class Solution {
public:
    int leastBricks(vector<vector<int>>& wall) {
        unordered_map<int, int> edge_counts;
        int max_count = 0;
        for (const auto& row : wall) {
            int current_sum = 0;
            for (int i = 0; i < row.size() - 1; ++i) {
                current_sum += row[i];
                edge_counts[current_sum]++;
                if (edge_counts[current_sum] > max_count) {
                    max_count = edge_counts[current_sum];
                }
            }
        }
        return wall.size() - max_count;
    }
};
