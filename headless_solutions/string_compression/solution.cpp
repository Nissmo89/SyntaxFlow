class Solution {
public:
    int compress(vector<char>& chars) {
        int write = 0, read = 0;
        while (read < chars.size()) {
            char currentChar = chars[read];
            int count = 0;
            while (read < chars.size() && chars[read] == currentChar) {
                read++;
                count++;
            }
            chars[write++] = currentChar;
            if (count > 1) {
                string s = to_string(count);
                for (char c : s) chars[write++] = c;
            }
        }
        return write;
    }
};
