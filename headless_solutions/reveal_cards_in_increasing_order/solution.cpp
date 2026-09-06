#include <vector>
#include <algorithm>
#include <deque>

class Solution {
public:
    std::vector<int> deckRevealedIncreasing(std::vector<int>& deck) {
        std::sort(deck.rbegin(), deck.rend());
        std::deque<int> dq;
        
        for (int card : deck) {
            if (!dq.empty()) {
                dq.push_front(dq.back());
                dq.pop_back();
            }
            dq.push_front(card);
        }
        
        return std::vector<int>(dq.begin(), dq.end());
    }
};
