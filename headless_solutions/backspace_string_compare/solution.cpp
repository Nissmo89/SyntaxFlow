class Solution {
public:
    bool backspaceCompare(string s, string t) {
        int i = s.length() - 1; // Pointer for string s, starting from the end
        int j = t.length() - 1; // Pointer for string t, starting from the end
        int skipS = 0;          // Counter for backspaces in s
        int skipT = 0;          // Counter for backspaces in t

        // Loop until both strings are fully processed (pointers go below 0)
        while (i >= 0 || j >= 0) {
            // Process string s: find the next valid character from the end
            while (i >= 0) {
                if (s[i] == '#') {
                    skipS++; // Found a backspace, increment skip counter
                    i--;     // Move to the previous character
                } else if (skipS > 0) {
                    skipS--; // This character is skipped by a previous backspace
                    i--;     // Move to the previous character
                } else {
                    break;   // Found a valid character, break inner loop
                }
            }

            // Process string t: find the next valid character from the end
            while (j >= 0) {
                if (t[j] == '#') {
                    skipT++; // Found a backspace, increment skip counter
                    j--;     // Move to the previous character
                } else if (skipT > 0) {
                    skipT--; // This character is skipped by a previous backspace
                    j--;     // Move to the previous character
                } else {
                    break;   // Found a valid character, break inner loop
                }
            }

            // Now, i and j point to the next effective characters (or are < 0)
            // Compare the characters
            if (i >= 0 && j >= 0 && s[i] == t[j]) {
                // Both strings have valid characters and they match
                i--; // Move to the previous character in s
                j--; // Move to the previous character in t
            } else {
                // Mismatch, or one string exhausted while the other still has valid characters
                // If both i and j are -1, it means both strings became empty and matched.
                // Otherwise, they don't match.
                return i == -1 && j == -1;
            }
        }

        // If the loop finishes, it means both i and j have become -1,
        // implying all effective characters matched.
        return true;
    }
};
