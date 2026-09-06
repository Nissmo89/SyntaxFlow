class Solution {
public:
    vector<string> commonChars(vector<string>& words) {
        vector<int> freq(26, 0);
        // Initialize freq with the first word
        for (char c : words[0]) {
            freq[c - 'a']++;
        }
        
        // Update freq for each subsequent word
        for (int i = 1; i < words.size(); i++) {
            vector<int> temp_freq(26, 0);
            for (char c : words[i]) {
                temp_freq[c - 'a']++;
            }
            for (int j = 0; j < 26; j++) {
                freq[j] = min(freq[j], temp_freq[j]);
            }
        }
        
        // Construct the result
        vector<string> result;
        for (int i = 0; i < 26; i++) {
            while (freq[i] > 0) {
                result.push_back(string(1, 'a' + i));
                freq[i]--;
            }
        }
        return result;
    }
};
