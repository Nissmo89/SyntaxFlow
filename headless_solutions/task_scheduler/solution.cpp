class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);
        int maxFreq = 0;
        for (char t : tasks) {
            freq[t - 'A']++;
            maxFreq = max(maxFreq, freq[t - 'A']);
        }
        
        int countMax = 0;
        for (int f : freq) {
            if (f == maxFreq) countMax++;
        }
        
        int result = (maxFreq - 1) * (n + 1) + countMax;
        return max((int)tasks.size(), result);
    }
};
