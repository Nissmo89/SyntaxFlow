#include <string>
#include <vector>
#include <algorithm> // For std::max

class Solution {
public:
    int compareVersion(std::string version1, std::string version2) {
        int p1 = 0; // Pointer for version1
        int p2 = 0; // Pointer for version2
        int n1 = version1.length();
        int n2 = version2.length();

        // Iterate as long as there are characters to process in either string
        while (p1 < n1 || p2 < n2) {
            long long num1 = 0; // Use long long to avoid potential overflow during intermediate calculations, though problem states 32-bit int is sufficient for final values.
            long long num2 = 0;

            // Parse the current revision from version1
            while (p1 < n1 && version1[p1] != '.') {
                num1 = num1 * 10 + (version1[p1] - '0');
                p1++;
            }

            // Parse the current revision from version2
            while (p2 < n2 && version2[p2] != '.') {
                num2 = num2 * 10 + (version2[p2] - '0');
                p2++;
            }

            // Compare the parsed revisions
            if (num1 > num2) {
                return 1;
            }
            if (num1 < num2) {
                return -1;
            }

            // Move past the dot if it exists
            if (p1 < n1) {
                p1++; // Skip the '.'
            }
            if (p2 < n2) {
                p2++; // Skip the '.'
            }
        }

        // If all revisions were equal
        return 0;
    }
};
