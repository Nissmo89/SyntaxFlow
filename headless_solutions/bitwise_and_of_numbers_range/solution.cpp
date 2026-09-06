class Solution {
public:
    int rangeBitwiseAnd(int left, int right) {
        int m = left, n = right;
        int moveFactor = 1;
        while (m != n) {
            m >>= 1;
            n >>= 1;
            moveFactor <<= 1;
        }
        return m * moveFactor;
    }
};
