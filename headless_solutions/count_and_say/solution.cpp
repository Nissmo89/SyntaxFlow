#include <string>
#include <vector>

class Solution {
public:
    string countAndSay(int n) {
        string res = "1";
        for (int i = 1; i < n; ++i) {
            string next = "";
            for (int j = 0; j < res.length(); ++j) {
                int count = 1;
                while (j + 1 < res.length() && res[j] == res[j + 1]) {
                    count++;
                    j++;
                }
                next += to_string(count) + res[j];
            }
            res = next;
        }
        return res;
    }
};
