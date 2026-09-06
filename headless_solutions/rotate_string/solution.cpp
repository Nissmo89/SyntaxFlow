class Solution {
public:
    bool rotateString(string s, string goal) {
        if (s.size() != goal.size()) return false;
        int length = s.size();
        for (int i = 0; i < length; i++) {
            string rotated = s;
            for (int j = 0; j < length - i; j++) {
                rotated[j] = rotated[j + 1];
            }
            rotated[length - i] = s[0];
            if (rotated == goal) return true;
        }
        return false;
    }
};
