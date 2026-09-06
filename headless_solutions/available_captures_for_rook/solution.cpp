#include <vector> // Required for std::vector

class Solution {
public:
    int numRookCaptures(std::vector<std::vector<char>>& board) {
        int rook_row = -1;
        int rook_col = -1;

        // Step 1: Find the rook's position
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (board[r][c] == 'R') {
                    rook_row = r;
                    rook_col = c;
                    break; // Found the rook, no need to search further in this row
                }
            }
            if (rook_row != -1) { // Check if rook was found in the current row
                break; // Found the rook, no need to search further in other rows
            }
        }

        int captures = 0;
        // Step 3: Define directions (up, down, left, right)
        // dr: change in row, dc: change in column
        int dr[] = {-1, 1, 0, 0}; // Changes for row: Up, Down, No change, No change
        int dc[] = {0, 0, -1, 1}; // Changes for col: No change, No change, Left, Right

        // Step 4: Simulate movement for each direction
        for (int i = 0; i < 4; ++i) {
            int current_dr = dr[i];
            int current_dc = dc[i];

            // Start from the rook's position for each new direction
            int current_row = rook_row;
            int current_col = rook_col;

            // Move step by step in the current direction
            while (true) {
                current_row += current_dr;
                current_col += current_dc;

                // Boundary Check: If outside the board, stop searching in this direction
                if (current_row < 0 || current_row >= 8 || current_col < 0 || current_col >= 8) {
                    break;
                }

                char piece = board[current_row][current_col];

                // Check the cell content
                if (piece == 'p') {
                    captures++; // Found a pawn, capture it
                    break;      // Path is blocked by the pawn, stop in this direction
                } else if (piece == 'B') {
                    break;      // Found a bishop, path is blocked, no capture
                }
                // If piece == '.', continue to the next square in this direction
            }
        }

        return captures;
    }
};
