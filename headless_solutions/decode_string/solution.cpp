class Solution {
public:
    string decodeString(string s) {
        stack<int> countStack;
        stack<string> stringStack;
        string curString = "";
        int curNum = 0;
        
        for (char c : s) {
            if (isdigit(c)) {
                curNum = curNum * 10 + (c - '0');
            } else if (c == '[') {
                countStack.push(curNum);
                stringStack.push(curString);
                curNum = 0;
                curString = "";
            } else if (c == ']') {
                int repeat = countStack.top(); countStack.pop();
                string prev = stringStack.top(); stringStack.pop();
                for (int i = 0; i < repeat; ++i) prev += curString;
                curString = prev;
            } else {
                curString += c;
            }
        }
        return curString;
    }
};
