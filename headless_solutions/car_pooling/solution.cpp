#include <vector>

class Solution {
public:
    bool carPooling(std::vector<std::vector<int>>& trips, int capacity) {
        // Since the maximum location is 1000, we use a fixed-size array.
        // stops[i] stores the net change of passengers at location i.
        int stops[1001] = {0};
        
        for (const auto& trip : trips) {
            int numPassengers = trip[0];
            int from = trip[1];
            int to = trip[2];
            
            stops[from] += numPassengers;
            stops[to] -= numPassengers;
        }
        
        int currentPassengers = 0;
        for (int i = 0; i < 1001; ++i) {
            currentPassengers += stops[i];
            // If at any point passengers exceed capacity, return false.
            if (currentPassengers > capacity) {
                return false;
            }
        }
        
        return true;
    }
};
