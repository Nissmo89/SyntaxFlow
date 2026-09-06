#include <vector>
#include <map>
#include <algorithm>

class Solution {
    std::map<std::vector<int>, int> memo;
public:
    int shoppingOffers(std::vector<int>& price, std::vector<std::vector<int>>& special, std::vector<int>& needs) {
        if (memo.count(needs)) return memo[needs];
        
        int min_cost = 0;
        for (int i = 0; i < needs.size(); ++i) min_cost += needs[i] * price[i];
        
        for (auto& offer : special) {
            std::vector<int> next_needs = needs;
            bool possible = true;
            for (int i = 0; i < needs.size(); ++i) {
                if (next_needs[i] < offer[i]) { possible = false; break; }
                next_needs[i] -= offer[i];
            }
            if (possible) {
                min_cost = std::min(min_cost, offer.back() + shoppingOffers(price, special, next_needs));
            }
        }
        return memo[needs] = min_cost;
    }
};
