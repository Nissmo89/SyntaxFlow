#include <vector>
#include <stack>
#include <unordered_set>

class Solution {
public:
    bool canVisitAllRooms(std::vector<std::vector<int>>& rooms) {
        int n = rooms.size();
        std::unordered_set<int> visited = {0};
        std::stack<int> s;
        s.push(0);
        
        while (!s.empty()) {
            int curr = s.top();
            s.pop();
            for (int key : rooms[curr]) {
                if (visited.find(key) == visited.end()) {
                    visited.insert(key);
                    s.push(key);
                }
            }
        }
        return visited.size() == n;
    }
};
