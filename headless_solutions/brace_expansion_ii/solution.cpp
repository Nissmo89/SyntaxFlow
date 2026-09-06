#include <vector>
#include <string>
#include <set>
#include <stack>
#include <algorithm>

class Solution {
public:
    vector<string> braceExpansionII(string expression) {
        stack<vector<set<string>>> st;
        vector<set<string>> res;
        set<string> cur;

        for (char c : expression) {
            if (isalpha(c)) {
                if (cur.empty()) cur.insert(string(1, c));
                else {
                    set<string> next;
                    for (const string& s : cur) next.insert(s + c);
                    cur = next;
                }
            } else if (c == '{') {
                st.push(res);
                st.push({cur});
                res.clear();
                cur.clear();
            } else if (c == '}') {
                res.push_back(cur);
                set<string> combined;
                for (auto& s : res) combined.insert(s.begin(), s.end());
                
                set<string> prev_cur = *st.top().begin(); st.pop();
                vector<set<string>> prev_res = st.top(); st.pop();
                
                cur.clear();
                if (prev_cur.empty()) cur = combined;
                else {
                    for (const string& p : prev_cur)
                        for (const string& c : combined) cur.insert(p + c);
                }
                res = prev_res;
            } else if (c == ',') {
                res.push_back(cur);
                cur.clear();
            }
        }
        res.push_back(cur);
        set<string> final_set;
        for (auto& s : res) final_set.insert(s.begin(), s.end());
        return vector<string>(final_set.begin(), final_set.end());
    }
};
