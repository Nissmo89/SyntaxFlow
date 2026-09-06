class Solution {
public:
    int calculate(string s) {
        int len;
        if (s == "" || (len = s.length()) == 0) return 0;
        stack<int> stack;
        int num = 0;
        char sign = '+';
        for (int i = 0; i < len; i++) {
            if (isdigit(s[i])) {
                num = num * 10 + s[i] - '0';
            }
            if ((!isdigit(s[i]) && s[i] != ' ') || i == len - 1) {
                if (sign == '-') {
                    stack.push(-num);
                }
                if (sign == '+') {
                    stack.push(num);
                }
                if (sign == '*') {
                    stack.push(stack.top() * num);
                    stack.pop();
                }
                if (sign == '/') {
                    stack.push(stack.top() / num);
                    stack.pop();
                }
                sign = s[i];
                num = 0;
            }
        }

        int re = 0;
        while (!stack.empty()) {
            re += stack.top();
            stack.pop();
        }
        return re;
    }
};
