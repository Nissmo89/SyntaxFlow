#include <vector>
#include <queue>

class Solution {
public:
    int minRefuelStops(int target, int startFuel, std::vector<std::vector<int>>& stations) {
        // Max-heap to store available fuel from passed stations
        std::priority_queue<int> maxHeap;
        
        long long currReach = startFuel; // Use 64-bit int to prevent overflow
        int stops = 0;
        int i = 0;
        int n = stations.size();

        while (currReach < target) {
            // Push all gas stations reachable with current fuel into heap
            while (i < n && stations[i][0] <= currReach) {
                maxHeap.push(stations[i][1]);
                i++;
            }

            // If no reachable gas stations are available, target cannot be reached
            if (maxHeap.empty()) {
                return -1;
            }

            // Refuel using the station with the largest available gas
            currReach += maxHeap.top();
            maxHeap.pop();
            stops++;
        }

        return stops;
    }
};
