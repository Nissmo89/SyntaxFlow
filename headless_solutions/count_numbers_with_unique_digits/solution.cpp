class Solution {
public:
    int countNumbersWithUniqueDigits(int n) {
        // Base case for 10^0 = 1, range is 0 <= x < 1 (only x = 0)
        if (n == 0) {
            return 1;
        }
        
        int total = 10;            // Base answer for n = 1 (digits 0 through 9)
        int uniqueDigits = 9;     // Unique choices for the first digit (1..9)
        int availableOptions = 9; // Available digits for subsequent positions
        
        for (int i = 2; i <= n && availableOptions > 0; ++i) {
            uniqueDigits *= availableOptions;
            total += uniqueDigits;
            availableOptions--;
        }
        
        return total;
    }
};
