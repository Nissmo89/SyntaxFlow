class Solution {
public:
    bool isValid(string s) {
        vector<char> stack;
        for (char c : s) {
            if (c == 'c') {
                if (stack.size() >= 2 && stack.back() == 'b' && stack[stack.size() - 2] == 'a') {
                    stack.pop_back();
                    stack.pop_back();
                } else {
                    return false;
                }
            } else {
                stack.push_back(c);
            }
        }
        return stack.empty();
    }
};
