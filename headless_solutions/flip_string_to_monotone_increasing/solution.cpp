class Solution {
public:
    int minFlipsMonoIncr(string s) {
        int count_one = 0;
        int count_flip = 0;
        for (char c : s) {
            if (c == '1') {
                count_one++;
            } else {
                // Option 1: Flip this '0' to '1' (count_flip + 1)
                // Option 2: Flip all previous '1's to '0's (count_one)
                count_flip = std::min(count_flip + 1, count_one);
            }
        }
        return count_flip;
    }
};
