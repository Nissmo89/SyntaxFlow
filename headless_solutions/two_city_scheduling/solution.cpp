class Solution {
public:
    int twoCitySchedCost(vector<vector<int>>& costs) {
        // Sort based on the difference (costB - costA)
        sort(costs.begin(), costs.end(), [](const vector<int>& a, const vector<int>& b) {
            return (a[1] - a[0]) < (b[1] - b[0]);
        });
        
        int n = costs.size() / 2;
        int totalCost = 0;
        for (int i = 0; i < n; ++i) totalCost += costs[i][1]; // First half to B
        for (int i = n; i < 2 * n; ++i) totalCost += costs[i][0]; // Second half to A
        return totalCost;
    }
};
