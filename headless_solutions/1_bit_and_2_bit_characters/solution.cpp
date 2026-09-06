// C++ implementation matching signature
class Solution {
public:
    bool isOneBitCharacter(vector<int>& bits) {
        int i = 0;
        while (i < (int)bits.size() - 1) {
            if (bits[i] == 0) {
                ++i;          // 1‑bit character
            } else {
                i += 2;       // 2‑bit character
            }
        }
        return i == (int)bits.size() - 1;
    }
};
