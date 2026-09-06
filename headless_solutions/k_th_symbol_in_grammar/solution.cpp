class Solution {
public:
    int kthGrammar(int n, int k) {
        if (n == 1) return 0;
        // If k is in the second half, it's the inverse of the corresponding element in the first half
        if (k > (1 << (n - 2))) {
            return 1 - kthGrammar(n - 1, k - (1 << (n - 2)));
        }
        return kthGrammar(n - 1, k);
    }
};
