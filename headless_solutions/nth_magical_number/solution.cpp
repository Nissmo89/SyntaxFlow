class Solution {
public:
    int nthMagicalNumber(int n, int a, int b) {
        long long A = a, B = b;
        long long lcm = (A * B) / std::gcd(A, B);
        long long left = 2, right = 4e13; // Sufficient upper bound
        long long mod = 1e9 + 7;
        
        while (left < right) {
            long long mid = left + (right - left) / 2;
            if (mid / A + mid / B - mid / lcm < n) left = mid + 1;
            else right = mid;
        }
        return (int)(left % mod);
    }
};
