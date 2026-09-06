var numComponents = function(head, nums) {
    const set = new Set(nums);
    let count = 0;
    let curr = head;
    
    while (curr !== null) {
        if (set.has(curr.val) && (curr.next === null || !set.has(curr.next.val))) {
            count++;
        }
        curr = curr.next;
    }
    return count;
};
