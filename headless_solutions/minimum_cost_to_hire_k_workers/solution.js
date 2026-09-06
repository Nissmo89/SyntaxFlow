/**
 * @param {number[]} quality
 * @param {number[]} wage
 * @param {number} k
 * @return {number}
 */
var mincostToHireWorkers = function(quality, wage, k) {
    const n = quality.length;
    const workers = [];
    for (let i = 0; i < n; i++) {
        workers.push({ ratio: wage[i] / quality[i], quality: quality[i] });
    }
    
    // Sort workers by wage-to-quality ratio ascending
    workers.sort((a, b) => a.ratio - b.ratio);
    
    // Self-contained Max Heap implementation for standalone runtime
    class MaxHeap {
        constructor() {
            this.heap = [];
        }
        push(val) {
            this.heap.push(val);
            this._siftUp(this.heap.length - 1);
        }
        pop() {
            if (this.heap.length === 0) return null;
            const top = this.heap[0];
            const bottom = this.heap.pop();
            if (this.heap.length > 0) {
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
                const parent = (i - 1) >> 1;
                if (this.heap[parent] >= this.heap[i]) break;
                [this.heap[parent], this.heap[i]] = [this.heap[i], this.heap[parent]];
                i = parent;
            }
        }
        _siftDown(i) {
            const len = this.heap.length;
            while ((i << 1) + 1 < len) {
                let left = (i << 1) + 1;
                let right = left + 1;
                let maxIdx = i;
                if (left < len && this.heap[left] > this.heap[maxIdx]) maxIdx = left;
                if (right < len && this.heap[right] > this.heap[maxIdx]) maxIdx = right;
                if (maxIdx === i) break;
                [this.heap[i], this.heap[maxIdx]] = [this.heap[maxIdx], this.heap[i]];
                i = maxIdx;
            }
        }
    }
    
    const maxHeap = new MaxHeap();
    let qualitySum = 0;
    let minCost = Infinity;
    
    for (let i = 0; i < n; i++) {
        const { ratio, quality: q } = workers[i];
        qualitySum += q;
        maxHeap.push(q);
        
        if (maxHeap.size() > k) {
            qualitySum -= maxHeap.pop();
        }
        
        if (maxHeap.size() === k) {
            minCost = Math.min(minCost, ratio * qualitySum);
        }
    }
    
    return minCost;
};
