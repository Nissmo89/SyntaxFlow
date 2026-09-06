class Solution {
public:
    int findLUSlength(string a, string b) {
        // If a and b are equal, return -1
        if (a == b) {
            return -1;
        }
        // Otherwise, return the maximum length of a and b
        return max(a.length(), b.length());
    }
};
