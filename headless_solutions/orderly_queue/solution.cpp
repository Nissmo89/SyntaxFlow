#include <string>
#include <algorithm>
#include <vector>

class Solution {
public:
    string orderlyQueue(string s, int k) {
        if (k > 1) {
            sort(s.begin(), s.end());
            return s;
        }
        string res = s;
        for (int i = 1; i < s.length(); ++i) {
            string rotated = s.substr(i) + s.substr(0, i);
            if (rotated < res) res = rotated;
        }
        return res;
    }
};
