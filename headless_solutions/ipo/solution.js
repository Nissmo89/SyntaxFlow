/**
 * @param {number} k
 * @param {number} w
 * @param {number[]} profits
 * @param {number[]} capital
 * @return {number}
 */
var findMaximizedCapital = function(k, w, profits, capital) {
    const n = profits.length;

    // Custom MaxHeap implementation for JavaScript
    class MaxHeap {
        constructor() {
            this.heap = [];
        }

        push(val) {
            this.heap.push(val);
            this._bubbleUp();
        }

        pop() {
            if (this.isEmpty()) {
                return null;
            }
            if (this.heap.length === 1) {
                return this.heap.pop();
            }
            const max = this.heap[0];
            this.heap[0] = this.heap.pop();
            this._bubbleDown();
            return max;
        }

        peek() {
            return this.isEmpty() ? null : this.heap[0];
        }

        isEmpty() {
            return this.heap.length === 0;
        }

        _bubbleUp() {
            let index = this.heap.length - 1;
            while (index > 0) {
                let parentIndex = Math.floor((index - 1) / 2);
                if (this.heap[parentIndex] >= this.heap[index]) {
                    break;
                }
                [this.heap[parentIndex], this.heap[index]] = [this.heap[index], this.heap[parentIndex]];
                index = parentIndex;
            }
        }

        _bubbleDown() {
            let index = 0;
            const lastIndex = this.heap.length - 1;
            while (true) {
                let leftChildIndex = 2 * index + 1;
                let rightChildIndex = 2 * index + 2;
                let largestIndex = index;

                if (leftChildIndex <= lastIndex && this.heap[leftChildIndex] > this.heap[largestIndex]) {
                    largestIndex = leftChildIndex;
                }
                if (rightChildIndex <= lastIndex && this.heap[rightChildIndex] > this.heap[largestIndex]) {
                    largestIndex = rightChildIndex;
                }

                if (largestIndex === index) {
                    break;
                }

                [this.heap[index], this.heap[largestIndex]] = [this.heap[largestIndex], this.heap[index]];
                index = largestIndex;
            }
        }
    }

    // Combine projects into an array of [capital, profit] arrays
    const projects = [];
    for (let i = 0; i < n; i++) {
        projects.push([capital[i], profits[i]]);
    }

    // Sort projects by their capital requirements in ascending order
    projects.sort((a, b) => a[0] - b[0]);

    const maxProfitHeap = new MaxHeap();
    let currentCapital = w;
    let projectIdx = 0; // Pointer for the sorted projects array

    // Perform at most k projects
    for (let i = 0; i < k; i++) {
        // Add all affordable projects to the max-profit heap
        while (projectIdx < n && projects[projectIdx][0] <= currentCapital) {
            maxProfitHeap.push(projects[projectIdx][1]);
            projectIdx++;
        }

        // If no projects are affordable, we can't do any more.
        if (maxProfitHeap.isEmpty()) {
            break;
        }

        // Pick the project with the maximum profit from the heap
        currentCapital += maxProfitHeap.pop();
    }

    return currentCapital;
};
