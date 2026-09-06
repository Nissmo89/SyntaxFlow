class Solution {
public:
    bool validUtf8(vector<int>& data) {
        int remaining = 0;
        for (int val : data) {
            if (remaining == 0) {
                if ((val >> 7) == 0) continue;
                else if ((val >> 5) == 0b110) remaining = 1;
                else if ((val >> 4) == 0b1110) remaining = 2;
                else if ((val >> 3) == 0b11110) remaining = 3;
                else return false;
            } else {
                if ((val >> 6) != 0b10) return false;
                remaining--;
            }
        }
        return remaining == 0;
    }
};
