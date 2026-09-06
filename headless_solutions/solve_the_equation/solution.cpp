#include <string>
#include <vector>
#include <sstream>

class Solution {
public:
    string solveEquation(string equation) {
        int x = 0, num = 0, sign = 1, side = 1;
        for (int i = 0; i < equation.size(); ++i) {
            if (equation[i] == '=') {
                sign = 1; side = -1;
            } else if (equation[i] == '+' || equation[i] == '-') {
                sign = (equation[i] == '+') ? 1 : -1;
            } else {
                int j = i;
                while (j < equation.size() && equation[j] != '+' && equation[j] != '-' && equation[j] != '=') j++;
                string sub = equation.substr(i, j - i);
                if (sub.back() == 'x') {
                    string val = sub.substr(0, sub.size() - 1);
                    x += side * sign * (val.empty() || val == "+" ? 1 : (val == "-" ? -1 : stoi(val)));
                } else {
                    num += side * sign * stoi(sub);
                }
                i = j - 1;
            }
        }
        if (x == 0) return num == 0 ? "Infinite solutions" : "No solution";
        return "x=" + to_string(-num / x);
    }
};
