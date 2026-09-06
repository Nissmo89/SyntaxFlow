#include <vector>
#include <string>
#include <algorithm>

class Solution {
public:
    string largestNumber(vector<int>& nums) {
        vector<string> strs;
        for (int num : nums) strs.push_back(to_string(num));
        
        sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
            return a + b > b + a;
        });
        
        if (strs[0] == "0") return "0";
        
        string res = "";
        for (const string& s : strs) res += s;
        return res;
    }
};
