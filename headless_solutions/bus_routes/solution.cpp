class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if (source == target) return 0;
        unordered_map<int, vector<int>> stop_to_routes;
        for (int i = 0; i < routes.size(); ++i) {
            for (int stop : routes[i]) stop_to_routes[stop].push_back(i);
        }
        queue<pair<int, int>> q;
        q.push({source, 0});
        unordered_set<int> visited_stops = {source};
        unordered_set<int> visited_routes;
        
        while (!q.empty()) {
            auto [curr_stop, count] = q.front(); q.pop();
            for (int route_idx : stop_to_routes[curr_stop]) {
                if (visited_routes.count(route_idx)) continue;
                visited_routes.insert(route_idx);
                for (int next_stop : routes[route_idx]) {
                    if (next_stop == target) return count + 1;
                    if (!visited_stops.count(next_stop)) {
                        visited_stops.insert(next_stop);
                        q.push({next_stop, count + 1});
                    }
                }
            }
        }
        return -1;
    }
};
