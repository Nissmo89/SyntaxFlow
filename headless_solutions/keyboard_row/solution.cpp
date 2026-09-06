class Solution {
public:
    vector<string> findWords(vector<string>& words) {
        set<char> line1 = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'};
        set<char> line2 = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};
        set<char> line3 = {'z', 'x', 'c', 'v', 'b', 'n', 'm'};
        
        vector<string> result;
        for (const string& word : words) {
            set<char> w(word.begin(), word.end());
            if (w <= line1 || w <= line2 || w <= line3) {
                result.push_back(word);
            }
        }
        return result;
    }
};
