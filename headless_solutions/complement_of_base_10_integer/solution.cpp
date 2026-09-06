class Solution {
public:
    int bitwiseComplement(int n) {
        if (n == 0) return 1;
        int X = 1;
        while (n > X) X = (X << 1) + 1;
        return X ^ n;
    }
};
