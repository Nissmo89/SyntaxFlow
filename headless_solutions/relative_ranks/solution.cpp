class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& score) {
        int N = score.size();
        unordered_map<int, int> scoreToIndex;
        for (int i = 0; i < N; i++) {
            scoreToIndex[score[i]] = i;
        }
        vector<int> sortedScore = score;
        sort(sortedScore.begin(), sortedScore.end(), greater<int>());
        vector<string> answer(N);
        for (int i = 0; i < N; i++) {
            int index = scoreToIndex[sortedScore[i]];
            if (i == 0) {
                answer[index] = "Gold Medal";
            } else if (i == 1) {
                answer[index] = "Silver Medal";
            } else if (i == 2) {
                answer[index] = "Bronze Medal";
            } else {
                answer[index] = to_string(i + 1);
            }
        }
        return answer;
    }
};
