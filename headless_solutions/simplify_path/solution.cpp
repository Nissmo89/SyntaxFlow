#include <string>
#include <vector>
#include <sstream>

class Solution {
public:
    string simplifyPath(string path) {
        vector<string> stack;
        stringstream ss(path);
        string token;
        
        while (getline(ss, token, '/')) {
            if (token == "" || token == ".") continue;
            if (token == "..") {
                if (!stack.empty()) stack.pop_back();
            } else {
                stack.push_back(token);
            }
        }
        
        string res = "";
        for (const string& dir : stack) res += "/" + dir;
        return res.empty() ? "/" : res;
    }
};
