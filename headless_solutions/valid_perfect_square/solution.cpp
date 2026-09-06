class Solution {
public:
    bool isPerfectSquare(int num) {
        if (num < 2) return true;
        long long low = 2, high = num / 2;
        while (low <= high) {
            long long mid = (low + high) / 2;
            if (mid * mid == num) return true;
            else if (mid * mid < num) low = mid + 1;
            else high = mid - 1;
        }
        return false;
    }
};
