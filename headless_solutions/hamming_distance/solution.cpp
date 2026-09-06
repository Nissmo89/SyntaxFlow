class Solution {
public:
    int hammingDistance(int x, int y) {
        int result = x ^ y;
        int count = 0;
        while (result) {
            count++;
            result &= result - 1;
        }
        return count;
    }
};
