#include <vector>
#include <string>
#include <iostream> // For potential debugging, not strictly needed for solution

class Solution {
public:
    int minDeletionSize(std::vector<std::string>& strs) {
        // Handle edge case where input might be empty, though constraints say n >= 1
        if (strs.empty()) {
            return 0;
        }

        int deletedColumnsCount = 0;
        int numRows = strs.size();
        int numCols = strs[0].length(); // All strings have the same length

        // Iterate through each column
        for (int j = 0; j < numCols; ++j) {
            // For each column, iterate through rows to check if it's sorted
            // We compare strs[i][j] with strs[i+1][j]
            for (int i = 0; i < numRows - 1; ++i) {
                // If the current character is greater than the character below it,
                // the column is not sorted lexicographically.
                if (strs[i][j] > strs[i+1][j]) {
                    deletedColumnsCount++; // Increment count for this unsorted column
                    break; // No need to check further in this column, move to the next
                }
            }
        }

        return deletedColumnsCount;
    }
};
