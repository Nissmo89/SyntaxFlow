/**
 * @param {number[][]} points
 * @return {number}
 */
var maxPoints = function(points) {
    const n = points.length;
    if (n <= 2) return n;

    // Helper function for GCD
    const gcd = (a, b) => (b === 0 ? a : gcd(b, a % b));

    let maxOverall = 1;

    for (let i = 0; i < n; i++) {
        const slopeCounts = new Map();
        let localMax = 0;

        for (let j = i + 1; j < n; j++) {
            let dy = points[j][1] - points[i][1];
            let dx = points[j][0] - points[i][0];

            const g = gcd(Math.abs(dy), Math.abs(dx));
            dy /= g;
            dx /= g;

            // Normalize sign representation
            if (dx < 0 || (dx === 0 && dy < 0)) {
                dx = -dx;
                dy = -dy;
            }

            const key = `${dy}/${dx}`;
            const count = (slopeCounts.get(key) || 0) + 1;
            slopeCounts.set(key, count);
            localMax = Math.max(localMax, count);
        }

        maxOverall = Math.max(maxOverall, localMax + 1);
    }

    return maxOverall;
};
