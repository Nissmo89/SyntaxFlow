struct TrieNode {
    TrieNode* children[26] = {nullptr};
    bool isEnd = false;
};

class Solution {
    TrieNode* root = new TrieNode();
    void insert(string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            if (!curr->children[c - 'a']) curr->children[c - 'a'] = new TrieNode();
            curr = curr->children[c - 'a'];
        }
        curr->isEnd = true;
    }

public:
    string replaceWords(vector<string>& dictionary, string sentence) {
        for (string& w : dictionary) insert(w);
        stringstream ss(sentence), out;
        string word;
        bool first = true;
        while (ss >> word) {
            if (!first) out << " ";
            TrieNode* curr = root;
            string path = "";
            for (char c : word) {
                if (!curr->children[c - 'a'] || curr->isEnd) break;
                path += c;
                curr = curr->children[c - 'a'];
            }
            out << (curr->isEnd ? path : word);
            first = false;
        }
        return out.str();
    }
};
