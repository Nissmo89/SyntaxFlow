#include <vector>
#include <string>
#include <stack>

class Solution {
public:
    int evalRPN(std::vector<std::string>& tokens) {
        std::stack<int> s;
        for (const std::string& t : tokens) {
            if (t == "+" || t == "-" || t == "*" || t == "/") {
                int b = s.top(); s.pop();
                int a = s.top(); s.pop();
                if (t == "+") s.push(a + b);
                else if (t == "-") s.push(a - b);
                else if (t == "*") s.push(a * b);
                else s.push(a / b);
            } else {
                s.push(std::stoi(t));
            }
        }
        return s.top();
    }
};
