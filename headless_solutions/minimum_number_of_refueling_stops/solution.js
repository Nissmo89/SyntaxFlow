/**
 * Custom MaxHeap implementation for JavaScript
 */
class MaxHeap {
    constructor() {
        this.heap = [];
    }

    push(val) {
        this.heap.push(val);
        this._siftUp(this.heap.length - 1);
    }

    pop() {
        if (this.size() === 0) return null;
        const top = this.heap[0];
        const bottom = this.heap.pop();
        if (this.size() > 0) {
            this.heap[0] = bottom;
            this._siftDown(0);
        }
        return top;
    }

    size() {
        return this.heap.length;
    }

    _siftUp(i) {
        while (i > 0) {
            const p = Math.floor((i - 1) / 2);
            if (this.heap[p] >= this.heap[i]) break;
            [this.heap[p], this.heap[i]] = [this.heap[i], this.heap[p]];
            i = p;
        }
    }

    _siftDown(i) {
        const len = this.heap.length;
        while (i * 2 + 1 < len) {
            let left = i * 2 + 1, right = i * 2 + 2, maxIdx = left;
            if (right < len && this.heap[right] > this.heap[left]) {
                maxIdx = right;
            }
            if (this.heap[i] >= this.heap[maxIdx]) break;
            [this.heap[i], this.heap[maxIdx]] = [this.heap[maxIdx], this.heap[i]];
            i = maxIdx;
        }
    }
}

/**
 * @param {number} target
 * @param {number} startFuel
 * @param {number[][]} stations
 * @return {number}
 */
var minRefuelStops = function(target, startFuel, stations) {
    const maxHeap = new MaxHeap();
    let currReach = startFuel;
    let stops = 0;
    let i = 0;
    const n = stations.length;

    while (currReach < target) {
        while (i < n && stations[i][0] <= currReach) {
            maxHeap.push(stations[i][1]);
            i++;
        }

        if (maxHeap.size() === 0) {
            return -1;
        }

        currReach += maxHeap.pop();
        stops++;
    }

    return stops;
};
