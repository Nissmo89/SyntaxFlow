#include <vector>

class Solution {
public:
    std::vector<int> corpFlightBookings(std::vector<std::vector<int>>& bookings, int n) {
        std::vector<int> answer(n, 0);
        
        // Step 1: Record difference values at boundary positions
        for (const auto& booking : bookings) {
            int first = booking[0] - 1; // Convert 1-based index to 0-based
            int last = booking[1] - 1;
            int seats = booking[2];
            
            answer[first] += seats;
            if (last + 1 < n) {
                answer[last + 1] -= seats;
            }
        }
        
        // Step 2: Compute running prefix sum in-place
        for (int i = 1; i < n; ++i) {
            answer[i] += answer[i - 1];
        }
        
        return answer;
    }
};
