#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <deque>

class Solution {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        unordered_map<string, deque<string>> adj;
        for (auto& t : tickets) adj[t[0]].push_back(t[1]);
        for (auto& [u, neighbors] : adj) sort(neighbors.begin(), neighbors.end());
        
        vector<string> res;
        vector<string> stack = {"JFK"};
        while (!stack.empty()) {
            string curr = stack.back();
            if (!adj[curr].empty()) {
                stack.push_back(adj[curr].front());
                adj[curr].pop_front();
            } else {
                res.push_back(curr);
                stack.pop_back();
            }
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
