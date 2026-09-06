// C++ implementation
class Solution {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> res;
        for (int i = left; i <= right; i++) {
            if (isSelfDividing(i)) {
                res.push_back(i);
            }
        }
        return res;
    }

    bool isSelfDividing(int n) {
        for (int d = n; d > 0; d /= 10) {
            int lastDigit = d % 10;
            if (lastDigit == 0 || n % lastDigit != 0) {
                return false;
            }
        }
        return true;
    }
};
