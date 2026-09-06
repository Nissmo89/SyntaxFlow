class Solution {
public:
    int reverse(int n) {
        long long reversed = 0; // Use long long to handle potential overflow
        int sign = (n < 0) ? -1 : 1;
        n = abs(n);
        while (n > 0) {
            reversed = reversed * 10 + n % 10;
            n /= 10;
        }
        reversed *= sign;
        // Check for overflow
        if (reversed < INT_MIN || reversed > INT_MAX) {
            return 0;
        }
        return (int)reversed;
    }
};
