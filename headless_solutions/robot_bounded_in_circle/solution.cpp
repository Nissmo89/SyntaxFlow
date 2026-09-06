class Solution {
public:
    bool isRobotBounded(string instructions) {
        int x = 0, y = 0;
        int dx[] = {0, 1, 0, -1}; // N, E, S, W
        int dy[] = {1, 0, -1, 0};
        int dir = 0; // 0: N, 1: E, 2: S, 3: W

        for (char c : instructions) {
            if (c == 'G') {
                x += dx[dir];
                y += dy[dir];
            } else if (c == 'L') {
                dir = (dir + 3) % 4;
            } else {
                dir = (dir + 1) % 4;
            }
        }
        return (x == 0 && y == 0) || (dir != 0);
    }
};
