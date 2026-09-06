#include <vector>
#include <unordered_map>
#include <cstdlib>

class Solution {
private:
    std::unordered_map<int, std::vector<int>> index_map;

public:
    Solution(std::vector<int>& nums) {
        // Precompute indices for every target number
        for (int i = 0; i < nums.size(); ++i) {
            index_map[nums[i]].push_back(i);
        }
    }
    
    int pick(int target) {
        // Retrieve the vector of indices for target
        const auto& indices = index_map[target];
        // Pick a uniform random index
        int rand_pos = std::rand() % indices.size();
        return indices[rand_pos];
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(nums);
 * int param_1 = obj->pick(target);
 */
