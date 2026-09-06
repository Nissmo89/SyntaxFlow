#include <cmath>

class Solution {
public:
    int bulbSwitch(int n) {
        // sqrt returns double; casting to int truncates toward zero,
        // which is equivalent to floor for non‑negative n.
        return static_cast<int>(std::sqrt(n));
    }
};
