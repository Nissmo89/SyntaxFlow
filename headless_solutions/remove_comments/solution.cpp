class Solution {
public:
    vector<string> removeComments(vector<string>& source) {
        vector<string> res;
        string buffer;
        bool inBlock = false;
        for (const string& line : source) {
            for (int i = 0; i < line.size(); ++i) {
                if (inBlock) {
                    if (i + 1 < line.size() && line[i] == '*' && line[i + 1] == '/') {
                        inBlock = false;
                        i++;
                    }
                } else {
                    if (i + 1 < line.size() && line[i] == '/' && line[i + 1] == '*') {
                        inBlock = true;
                        i++;
                    } else if (i + 1 < line.size() && line[i] == '/' && line[i + 1] == '/') {
                        break;
                    } else {
                        buffer += line[i];
                    }
                }
            }
            if (!inBlock && !buffer.empty()) {
                res.push_back(buffer);
                buffer.clear();
            }
        }
        return res;
    }
};
