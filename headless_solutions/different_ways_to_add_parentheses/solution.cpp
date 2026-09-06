#include <vector>
#include <string>
#include <unordered_map>

class Solution {
    std::unordered_map<std::string, std::vector<int>> memo;
public:
    std::vector<int> diffWaysToCompute(std::string expression) {
        if (memo.count(expression)) return memo[expression];
        std::vector<int> res;
        for (int i = 0; i < expression.size(); ++i) {
            char c = expression[i];
            if (c == '+' || c == '-' || c == '*') {
                std::vector<int> left = diffWaysToCompute(expression.substr(0, i));
                std::vector<int> right = diffWaysToCompute(expression.substr(i + 1));
                for (int l : left) {
                    for (int r : right) {
                        if (c == '+') res.push_back(l + r);
                        else if (c == '-') res.push_back(l - r);
                        else res.push_back(l * r);
                    }
                }
            }
        }
        if (res.empty()) res.push_back(std::stoi(expression));
        return memo[expression] = res;
    }
};
