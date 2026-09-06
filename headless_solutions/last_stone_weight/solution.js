/**
 * @param {number[]} stones
 * @return {number}
 */
var lastStoneWeight = function (stones) {
    let pq = new MinPriorityQueue({ compare: (a, b) => b - a });
    for (let stone of stones) {
        pq.enqueue(stone);
    }
    while (pq.size() > 1) {
        let y = pq.dequeue();
        let x = pq.dequeue();
        if (x !== y) {
            pq.enqueue(y - x);
        }
    }
    return pq.size() === 0 ? 0 : pq.front().element;
};
