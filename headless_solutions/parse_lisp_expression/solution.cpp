#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

class Solution {
    int evaluate(string expression, unordered_map<string, int> scope) {
        if (expression[0] != '(') {
            if (isdigit(expression[0]) || expression[0] == '-') return stoi(expression);
            return scope[expression];
        }
        
        vector<string> tokens = parse(expression.substr(1, expression.size() - 2));
        if (tokens[0] == "add") return evaluate(tokens[1], scope) + evaluate(tokens[2], scope);
        if (tokens[0] == "mult") return evaluate(tokens[1], scope) * evaluate(tokens[2], scope);
        
        for (int i = 1; i < tokens.size() - 1; i += 2)
            scope[tokens[i]] = evaluate(tokens[i + 1], scope);
        return evaluate(tokens.back(), scope);
    }

    vector<string> parse(string s) {
        vector<string> res;
        int bal = 0;
        string cur;
        for (char c : s) {
            if (c == '(') bal++;
            if (c == ')') bal--;
            if (c == ' ' && bal == 0) { res.push_back(cur); cur = ""; }
            else cur += c;
        }
        res.push_back(cur);
        return res;
    }

public:
    int evaluate(string expression) {
        return evaluate(expression, {});
    }
};
