class Solution {
public:
    long long countTrailingZeroes(long long x) {
        long long count = 0;
        while (x >= 5) {
            count += x / 5;
            x /= 5;
        }
        return count;
    }

    int preimageSizeFZF(int k) {
        auto get_first = [&](int target) {
            long long low = 0, high = 5LL * target + 5;
            while (low < high) {
                long long mid = low + (high - low) / 2;
                if (countTrailingZeroes(mid) < target) low = mid + 1;
                else high = mid;
            }
            return low;
        };
        long long left = get_first(k);
        long long right = get_first(k + 1);
        return (int)(right - left);
    }
};
