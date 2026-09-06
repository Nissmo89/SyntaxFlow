#include <vector>
#include <string>
#include <queue>
#include <unordered_set>

class Solution {
public:
    int openLock(std::vector<std::string>& deadends, std::string target) {
        std::unordered_set<std::string> dead(deadends.begin(), deadends.end());
        if (dead.count("0000")) return -1;
        if (target == "0000") return 0;

        std::queue<std::string> q;
        q.push("0000");
        std::unordered_set<std::string> visited = {"0000"};
        int turns = 0;

        while (!q.empty()) {
            int size = q.size();
            while (size--) {
                std::string curr = q.front(); q.pop();
                for (int i = 0; i < 4; ++i) {
                    for (int delta : {-1, 1}) {
                        std::string next = curr;
                        next[i] = (next[i] - '0' + delta + 10) % 10 + '0';
                        if (next == target) return turns + 1;
                        if (!dead.count(next) && !visited.count(next)) {
                            visited.insert(next);
                            q.push(next);
                        }
                    }
                }
            }
            turns++;
        }
        return -1;
    }
};
