class Solution {
public:
    string removeDuplicateLetters(string s) {
        vector<int> count(26, 0);
        vector<bool> visited(26, false);
        for (char c : s) count[c - 'a']++;
        
        string stack = "";
        for (char c : s) {
            count[c - 'a']--;
            if (visited[c - 'a']) continue;
            
            while (!stack.empty() && c < stack.back() && count[stack.back() - 'a'] > 0) {
                visited[stack.back() - 'a'] = false;
                stack.pop_back();
            }
            stack.push_back(c);
            visited[c - 'a'] = true;
        }
        return stack;
    }
};
