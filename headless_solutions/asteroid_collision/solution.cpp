#include <vector>
#include <numeric> // For std::abs

class Solution {
public:
    std::vector<int> asteroidCollision(std::vector<int>& asteroids) {
        std::vector<int> result_stack; // Using a vector as a stack

        for (int asteroid : asteroids) {
            // Flag to track if the current asteroid has exploded
            bool asteroid_exploded = false;

            // Collision logic for negative asteroids
            // Only collide if stack is not empty, current asteroid is negative,
            // AND the top of the stack is positive (moving right).
            while (!result_stack.empty() && asteroid < 0 && result_stack.back() > 0) {
                int top_asteroid = result_stack.back(); // Get the top asteroid

                if (std::abs(asteroid) > top_asteroid) {
                    // Current negative asteroid is larger, top_asteroid explodes
                    result_stack.pop_back(); // Remove the exploded asteroid
                    // Current asteroid continues, so loop again to check new top
                } else if (std::abs(asteroid) == top_asteroid) {
                    // Both asteroids are same size, both explode
                    result_stack.pop_back(); // Remove the exploded top_asteroid
                    asteroid_exploded = true; // Current asteroid also explodes
                    break; // Stop checking for collisions for this asteroid
                } else { // std::abs(asteroid) < top_asteroid
                    // Current negative asteroid is smaller, it explodes
                    asteroid_exploded = true; // Current asteroid explodes
                    break; // Stop checking for collisions for this asteroid
                }
            }

            // If the current asteroid survived all collisions, push it to the stack
            if (!asteroid_exploded && asteroid != 0) { // asteroid != 0 is guaranteed by constraints, but good practice
                result_stack.push_back(asteroid);
            }
        }

        return result_stack;
    }
};
