#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> ans(n, 0);
        vector<int> stack;               // holds function ids
        int prev = 0;                     // timestamp of previous log

        for (const string& log : logs) {
            // split "id:type:time"
            int first = log.find(':');
            int second = log.find(':', first + 1);
            int id = stoi(log.substr(0, first));
            string type = log.substr(first + 1, second - first - 1);
            int time = stoi(log.substr(second + 1));

            if (type == "start") {
                if (!stack.empty()) {
                    ans[stack.back()] += time - prev;
                }
                stack.push_back(id);
                prev = time;
            } else { // "end"
                // the function on top finishes now
                ans[stack.back()] += time - prev + 1;
                stack.pop_back();
                prev = time + 1;
            }
        }
        return ans;
    }
};
