#include <vector>
#include <unordered_set>
#include <algorithm>

class Solution {
public:
    int robotSim(std::vector<int>& commands, std::vector<std::vector<int>>& obstacles) {
        std::unordered_set<long> obs;
        for (auto& o : obstacles) {
            obs.insert(((long)o[0] + 30000) * 60001 + (o[1] + 30000));
        }
        int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
        int x = 0, y = 0, d = 0, res = 0;
        for (int c : commands) {
            if (c == -1) d = (d + 1) % 4;
            else if (c == -2) d = (d + 3) % 4;
            else {
                while (c--) {
                    int nx = x + dx[d], ny = y + dy[d];
                    if (obs.count(((long)nx + 30000) * 60001 + (ny + 30000))) break;
                    x = nx; y = ny;
                    res = std::max(res, x * x + y * y);
                }
            }
        }
        return res;
    }
};
