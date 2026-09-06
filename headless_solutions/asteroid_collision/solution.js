/**
 * @param {number[]} asteroids
 * @return {number[]}
 */
var asteroidCollision = function(asteroids) {
    const resultStack = []; // Using an array as a stack

    for (const asteroid of asteroids) {
        // Flag to track if the current asteroid has exploded
        let asteroidExploded = false;

        // Collision logic for negative asteroids
        // Only collide if stack is not empty, current asteroid is negative,
        // AND the top of the stack is positive (moving right).
        while (resultStack.length > 0 && asteroid < 0 && resultStack[resultStack.length - 1] > 0) {
            const topAsteroid = resultStack[resultStack.length - 1]; // Get the top asteroid

            if (Math.abs(asteroid) > topAsteroid) {
                // Current negative asteroid is larger, topAsteroid explodes
                resultStack.pop(); // Remove the exploded asteroid
                // Current asteroid continues, so loop again to check new top
            } else if (Math.abs(asteroid) === topAsteroid) {
                // Both asteroids are same size, both explode
                resultStack.pop(); // Remove the exploded topAsteroid
                asteroidExploded = true; // Current asteroid also explodes
                break; // Stop checking for collisions for this asteroid
            } else { // Math.abs(asteroid) < topAsteroid
                // Current negative asteroid is smaller, it explodes
                asteroidExploded = true; // Current asteroid explodes
                break; // Stop checking for collisions for this asteroid
            }
        }

        // If the current asteroid survived all collisions, push it to the stack
        // The condition `asteroid != 0` is guaranteed by constraints.
        if (!asteroidExploded) {
            resultStack.push(asteroid);
        }
    }

    return resultStack;
};
