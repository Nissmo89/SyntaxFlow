#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<string>> findDuplicate(vector<string>& paths) {
        unordered_map<string, vector<string>> contentMap;   // content → list of full paths

        for (const string& line : paths) {
            stringstream ss(line);
            string dir;                     // first token = directory
            ss >> dir;

            string fileToken;
            while (ss >> fileToken) {       // each remaining token = "name(content)"
                size_t lparen = fileToken.find('(');
                size_t rparen = fileToken.find(')', lparen);
                string name = fileToken.substr(0, lparen);
                string content = fileToken.substr(lparen + 1, rparen - lparen - 1);
                string fullPath = dir + '/' + name;
                contentMap[content].push_back(fullPath);
            }
        }

        vector<vector<string>> result;
        for (auto& kv : contentMap) {
            if (kv.second.size() > 1)          // keep only duplicates
                result.push_back(move(kv.second));
        }
        return result;
    }
};
