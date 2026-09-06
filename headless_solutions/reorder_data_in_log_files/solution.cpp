class Solution {
public:
    vector<string> reorderLogFiles(vector<string>& logs) {
        vector<string> letters, digits;
        for (const string& log : logs) {
            int firstSpace = log.find(' ');
            if (isdigit(log[firstSpace + 1])) digits.push_back(log);
            else letters.push_back(log);
        }
        sort(letters.begin(), letters.end(), [](const string& a, const string& b) {
            int s1 = a.find(' '), s2 = b.find(' ');
            string content1 = a.substr(s1 + 1), content2 = b.substr(s2 + 1);
            if (content1 != content2) return content1 < content2;
            return a.substr(0, s1) < b.substr(0, s2);
        });
        letters.insert(letters.end(), digits.begin(), digits.end());
        return letters;
    }
};
