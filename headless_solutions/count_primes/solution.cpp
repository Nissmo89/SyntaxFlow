class Solution {
public:
    int countPrimes(int n) {
        if (n <= 2) return 0;
        
        // vector<bool> is space-optimized to 1 bit per element in C++
        vector<bool> isComposite(n, false);
        
        for (int i = 2; i * i < n; ++i) {
            if (!isComposite[i]) {
                // j starts at i * i to avoid redundant work
                for (int j = i * i; j < n; j += i) {
                    isComposite[j] = true;
                }
            }
        }
        
        int count = 0;
        for (int i = 2; i < n; ++i) {
            if (!isComposite[i]) {
                count++;
            }
        }
        
        return count;
    }
};
