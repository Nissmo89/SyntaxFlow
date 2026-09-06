#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <algorithm>

class Solution {
public:
    string countOfAtoms(string formula) {
        int n = formula.length(), i = 0;
        stack<map<string, int>> st;
        map<string, int> curr;
        
        while (i < n) {
            if (formula[i] == '(') {
                st.push(curr);
                curr.clear();
                i++;
            } else if (formula[i] == ')') {
                i++;
                int start = i;
                while (i < n && isdigit(formula[i])) i++;
                int mult = (i == start) ? 1 : stoi(formula.substr(start, i - start));
                map<string, int> prev = st.top(); st.pop();
                for (auto const& [atom, count] : curr) prev[atom] += count * mult;
                curr = prev;
            } else {
                int start = i++;
                while (i < n && islower(formula[i])) i++;
                string name = formula.substr(start, i - start);
                start = i;
                while (i < n && isdigit(formula[i])) i++;
                int count = (i == start) ? 1 : stoi(formula.substr(start, i - start));
                curr[name] += count;
            }
        }
        string res = "";
        for (auto const& [atom, count] : curr) {
            res += atom + (count > 1 ? to_string(count) : "");
        }
        return res;
    }
};
