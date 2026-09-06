#include <vector>
#include <string>
#include <numeric> // For std::accumulate, though not used with running sum
#include <iostream> // For stoi

class Solution {
public:
    int calPoints(std::vector<std::string>& operations) {
        std::vector<int> record; // Use a vector as a stack
        int totalSum = 0;

        for (const std::string& op : operations) {
            if (op == "+") {
                // Sum of previous two scores
                // Problem constraints guarantee at least two scores for '+'
                int score1 = record.back(); // Last score
                int score2 = record[record.size() - 2]; // Second last score
                int newScore = score1 + score2;
                record.push_back(newScore);
                totalSum += newScore;
            } else if (op == "D") {
                // Double of previous score
                // Problem constraints guarantee at least one score for 'D'
                int lastScore = record.back();
                int newScore = 2 * lastScore;
                record.push_back(newScore);
                totalSum += newScore;
            } else if (op == "C") {
                // Invalidate previous score
                // Problem constraints guarantee at least one score for 'C'
                int removedScore = record.back();
                record.pop_back(); // Remove the last score
                totalSum -= removedScore;
            } else {
                // An integer x
                int score = std::stoi(op); // Convert string to integer
                record.push_back(score);
                totalSum += score;
            }
        }

        return totalSum;
    }
};
