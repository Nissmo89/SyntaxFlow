#include <vector>
#include <unordered_map>

class Solution {
public:
    bool checkSubarraySum(std::vector<int>& nums, int k) {
        // Hash map to store {remainder: first_seen_index}
        std::unordered_map<int, int> seen;
        // Base case: prefix sum of 0 at virtual index -1
        seen[0] = -1;
        
        int prefixMod = 0;
        
        for (int i = 0; i < nums.size(); ++i) {
            prefixMod = (prefixMod + nums[i]) % k;
            
            if (seen.find(prefixMod) != seen.end()) {
                // Check if the subarray length is at least 2
                if (i - seen[prefixMod] >= 2) {
                    return true;
                }
            } else {
                // Store only the FIRST occurrence of this remainder
                seen[prefixMod] = i;
            }
        }
        
        return false;
    }
};
