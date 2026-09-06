class Solution {
public:
    int clumsy(int n) {
        if (n == 1) return 1;
        if (n == 2) return 2;
        if (n == 3) return 6;
        if (n == 4) return 7;
        int mod = n % 4;
        if (mod == 1 || mod == 2) return n + 2;
        else if (mod == 3) return n - 1;
        else return n + 1;
    }
};
