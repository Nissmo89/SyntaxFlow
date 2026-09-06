#include <cmath>

class Solution {
public:
    int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
        int rounds = minutesToTest / minutesToDie;
        int base = rounds + 1;
        int pigs = 0;
        int states = 1;
        while (states < buckets) {
            states *= base;
            pigs++;
        }
        return pigs;
    }
};
