class Solution {
public:
    int addDigits(int num) {
        // The digital root formula: 1 + (num - 1) % 9
        // This formula works for all non-negative integers.
        // Special cases:
        // - If num is 0: 1 + (0 - 1) % 9 = 1 + (-1) % 9 = 1 + (-1) = 0. Correct.
        // - If num is a positive multiple of 9 (e.g., 9, 18):
        //   1 + (9 - 1) % 9 = 1 + 8 % 9 = 1 + 8 = 9. Correct.
        // - For other positive numbers (e.g., 38):
        //   1 + (38 - 1) % 9 = 1 + 37 % 9 = 1 + 1 = 2. Correct.
        
        if (num == 0) {
            return 0;
        }
        return 1 + (num - 1) % 9;
    }
};
