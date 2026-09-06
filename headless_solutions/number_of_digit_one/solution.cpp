class Solution {
public:
    int countDigitOne(int n) {
        long long count = 0;
        for (long long m = 1; m <= n; m *= 10) {
            long long a = n / (m * 10);
            long long b = n % m;
            int x = (n / m) % 10;
            
            count += a * m;
            if (x > 1) count += m;
            else if (x == 1) count += (b + 1);
        }
        return (int)count;
    }
};
