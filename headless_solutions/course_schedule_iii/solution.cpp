#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class Solution {
public:
    int scheduleCourse(vector<vector<int>>& courses) {
        // Sort courses by their deadline (lastDay)
        sort(courses.begin(), courses.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        // Max-heap to store durations of courses taken so far
        priority_queue<int> pq;
        int totalTime = 0;

        for (const auto& course : courses) {
            int duration = course[0];
            int deadline = course[1];

            // Add the current course
            totalTime += duration;
            pq.push(duration);

            // If the current total time exceeds the deadline, 
            // remove the course with the largest duration to minimize totalTime.
            if (totalTime > deadline) {
                totalTime -= pq.top();
                pq.pop();
            }
        }

        return pq.size();
    }
};
