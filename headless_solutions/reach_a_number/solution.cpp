class Solution {
public:
    int reachNumber(int target) {
        target = std::abs(target);
        int step = 0, sum = 0;
        while (sum < target || (sum - target) % 2 != 0) {
            step++;
            sum += step;
        }
        return step;
    }
};
