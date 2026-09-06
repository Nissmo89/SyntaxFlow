/**
 * @param {number[][]} courses
 * @return {number}
 */
var scheduleCourse = function(courses) {
    // Sort courses by deadline
    courses.sort((a, b) => a[1] - b[1]);
    
    // JavaScript doesn't have a built-in Priority Queue, 
    // so we implement a simple Max-Heap or use a sorted array for small constraints.
    // For production/competitive programming, a Heap is required for O(N log N).
    // Here is a standard Max-Heap implementation logic:
    const maxHeap = new MaxHeap();
    let totalTime = 0;
    
    for (const [duration, deadline] of courses) {
        totalTime += duration;
        maxHeap.push(duration);
        
        if (totalTime > deadline) {
            totalTime -= maxHeap.pop();
        }
    }
    
    return maxHeap.size();
};

// Minimal Max-Heap implementation for JavaScript
class MaxHeap {
    constructor() {
        this.heap = [];
    }
    push(val) {
        this.heap.push(val);
        this.bubbleUp();
    }
    pop() {
        if (this.size() === 1) return this.heap.pop();
        const max = this.heap[0];
        this.heap[0] = this.heap.pop();
        this.bubbleDown();
        return max;
    }
    size() { return this.heap.length; }
    bubbleUp() {
        let index = this.heap.length - 1;
        while (index > 0) {
            let parentIndex = Math.floor((index - 1) / 2);
            if (this.heap[parentIndex] >= this.heap[index]) break;
            [this.heap[parentIndex], this.heap[index]] = [this.heap[index], this.heap[parentIndex]];
            index = parentIndex;
        }
    }
    bubbleDown() {
        let index = 0;
        while (true) {
            let left = 2 * index + 1;
            let right = 2 * index + 2;
            let largest = index;
            if (left < this.heap.length && this.heap[left] > this.heap[largest]) largest = left;
            if (right < this.heap.length && this.heap[right] > this.heap[largest]) largest = right;
            if (largest === index) break;
            [this.heap[index], this.heap[largest]] = [this.heap[largest], this.heap[index]];
            index = largest;
        }
    }
}
