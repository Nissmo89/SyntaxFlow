#include <vector>
#include <algorithm>

class Solution {
public:
    int rectangleArea(std::vector<std::vector<int>>& rectangles) {
        std::vector<std::vector<int>> events;
        for (auto& r : rectangles) {
            events.push_back({r[0], 1, r[1], r[3]});
            events.push_back({r[2], -1, r[1], r[3]});
        }
        std::sort(events.begin(), events.end());
        
        long long area = 0, mod = 1e9 + 7;
        std::vector<std::pair<int, int>> active;
        int prev_x = events[0][0];
        
        for (auto& e : events) {
            int x = e[0], type = e[1], y1 = e[2], y2 = e[3];
            long long width = x - prev_x;
            
            if (width > 0) {
                long long height = 0, last_y = -1;
                for (auto& interval : active) {
                    last_y = std::max(last_y, (long long)interval.first);
                    height += std::max(0LL, (long long)interval.second - last_y);
                    last_y = std::max(last_y, (long long)interval.second);
                }
                area = (area + width * height) % mod;
            }
            
            if (type == 1) active.push_back({y1, y2});
            else {
                for (auto it = active.begin(); it != active.end(); ++it) {
                    if (it->first == y1 && it->second == y2) {
                        active.erase(it);
                        break;
                    }
                }
            }
            std::sort(active.begin(), active.end());
            prev_x = x;
        }
        return (int)area;
    }
};
