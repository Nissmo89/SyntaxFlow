class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        string morseCode[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};
        unordered_set<string> seen;
        for (string word : words) {
            string code;
            for (char c : word) {
                code += morseCode[c - 'a'];
            }
            seen.insert(code);
        }
        return seen.size();
    }
};
