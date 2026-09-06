/**
 * @param {number[]} nums1
 * @param {number[]} nums2
 * @param {number} k
 * @return {number[][]}
 */
var kSmallestPairs = function(nums1, nums2, k) {
    const pq = new MinPriorityQueue({ priority: x => x.sum });
    for (let i = 0; i < Math.min(nums1.length, k); i++) {
        pq.enqueue({ sum: nums1[i] + nums2[0], i, j: 0 });
    }
    const res = [];
    while (k-- > 0 && !pq.isEmpty()) {
        const { sum, i, j } = pq.dequeue().element;
        res.push([nums1[i], nums2[j]]);
        if (j + 1 < nums2.length) {
            pq.enqueue({ sum: nums1[i] + nums2[j + 1], i, j: j + 1 });
        }
    }
    return res;
};
