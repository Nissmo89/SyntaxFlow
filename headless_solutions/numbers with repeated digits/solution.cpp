#include <vector>
#include <string>
#include <algorithm>

class Solution {
public:
    int numDupDigitsAtMostN(int n) {
        std::string s = std::to_string(n);
        int len = s.size();
        int count = 0;

        auto P = [](int n, int k) {
            int res = 1;
            for (int i = 0; i < k; ++i) res *= (n - i);
            return res;
        };

        // 1. Count numbers with fewer digits
        for (int i = 1; i < len; ++i)
            count += 9 * P(9, i - 1);

        // 2. Count numbers with same number of digits
        std::vector<bool> used(10, false);
        for (int i = 0; i < len; ++i) {
            int digit = s[i] - '0';
            for (int j = (i == 0 ? 1 : 0); j < digit; ++j) {
                if (!used[j]) count += P(9 - i, len - 1 - i);
            }
            if (used[digit]) return n - count;
            used[digit] = true;
        }
        return n - (count + 1);
    }
};
