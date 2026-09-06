/**
 * @param {number[][]} trees
 * @return {number[][]}
 */
var outerTrees = function(trees) {
    const n = trees.length;
    // If there are 3 or fewer trees, all are on the perimeter.
    if (n <= 3) {
        return trees;
    }

    // Helper function to compute the cross product of vectors OA and OB.
    // Returns positive for counter-clockwise turn, negative for clockwise, zero for collinear.
    const crossProduct = (O, A, B) => {
        // (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x)
        return (A[0] - O[0]) * (B[1] - O[1]) - (A[1] - O[1]) * (B[0] - O[0]);
    };

    // Sort points lexicographically: first by x, then by y.
    trees.sort((a, b) => {
        if (a[0] !== b[0]) {
            return a[0] - b[0];
        }
        return a[1] - b[1];
    });

    const lowerHull = [];
    const upperHull = [];

    // Build lower hull
    for (const tree of trees) {
        // While the last two points in lowerHull and the current tree make a clockwise turn (< 0)
        // We pop only on strict right turns to include collinear points on the boundary.
        while (lowerHull.length >= 2 && crossProduct(lowerHull[lowerHull.length - 2], lowerHull[lowerHull.length - 1], tree) < 0) {
            lowerHull.pop();
        }
        lowerHull.push(tree);
    }

    // Build upper hull
    // Iterate in reverse order
    for (let i = n - 1; i >= 0; i--) {
        const tree = trees[i];
        // Similar logic for upper hull, maintaining counter-clockwise turns when viewed from above.
        // Popping on strict right turns (< 0).
        while (upperHull.length >= 2 && crossProduct(upperHull[upperHull.length - 2], upperHull[upperHull.length - 1], tree) < 0) {
            upperHull.pop();
        }
        upperHull.push(tree);
    }

    // Combine hulls and remove duplicates.
    // Using a Map to store unique points, keyed by a string representation "x,y".
    const hullMap = new Map();

    // Add all points from lower hull
    for (const p of lowerHull) {
        hullMap.set(`${p[0]},${p[1]}`, p);
    }
    // Add all points from upper hull
    for (const p of upperHull) {
        hullMap.set(`${p[0]},${p[1]}`, p);
    }

    // Convert map values back to an array of arrays
    return Array.from(hullMap.values());
};
