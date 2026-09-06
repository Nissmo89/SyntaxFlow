class Solution {
public:
    int integerReplacement(int n) {
        long long num = n; // Use long long to prevent overflow when n = 2^31 - 1
        int count = 0;
        while (num > 1) {
            if (num % 2 == 0) {
                num /= 2;
            } else if (num == 3 || (num & 2) == 0) {
                // If num is 3 or ends in 01, subtract 1
                num--;
            } else {
                // If num ends in 11, add 1
                num++;
            }
            count++;
        }
        return count;
    }
};
