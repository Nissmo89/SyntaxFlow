#include <vector>
#include <string>
#include <unordered_set>
#include <queue>

class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (dict.find(endWord) == dict.end()) return 0;
        
        queue<string> q;
        q.push(beginWord);
        int depth = 1;
        
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                string curr = q.front(); q.pop();
                if (curr == endWord) return depth;
                
                for (int j = 0; j < curr.size(); ++j) {
                    char original = curr[j];
                    for (char c = 'a'; c <= 'z'; ++c) {
                        curr[j] = c;
                        if (dict.count(curr)) {
                            q.push(curr);
                            dict.erase(curr);
                        }
                    }
                    curr[j] = original;
                }
            }
            depth++;
        }
        return 0;
    }
};
