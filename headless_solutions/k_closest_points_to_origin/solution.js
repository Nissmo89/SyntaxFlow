/**
 * @param {number[][]} points
 * @param {number} k
 * @return {number[][]}
 */
var kClosest = function(points, k) {
    // Sort by distance and take the first k elements
    // Note: For production with large N, use a Max-Heap implementation.
    // Given JS lacks a built-in Heap, sorting is O(N log N).
    return points.sort((a, b) => {
        return (a[0]**2 + a[1]**2) - (b[0]**2 + b[1]**2);
    }).slice(0, k);
};
