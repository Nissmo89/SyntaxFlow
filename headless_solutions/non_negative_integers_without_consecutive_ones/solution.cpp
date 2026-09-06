class Solution {
public:
    int findIntegers(int n) {
        // f[i] stores the count of valid binary strings of length i
        int f[31];
        f[0] = 1;
        f[1] = 2;
        for (int i = 2; i < 31; ++i) {
            f[i] = f[i - 1] + f[i - 2];
        }

        int ans = 0;
        int prev_bit = 0;

        // Traverse from MSB (bit 30) down to LSB (bit 0)
        for (int k = 30; k >= 0; --k) {
            if ((n & (1 << k)) != 0) {
                ans += f[k];
                if (prev_bit == 1) {
                    return ans; // Terminate early if consecutive 1s are formed
                }
                prev_bit = 1;
            } else {
                prev_bit = 0;
            }
        }

        return ans + 1; // Include n itself if it's valid
    }
};
