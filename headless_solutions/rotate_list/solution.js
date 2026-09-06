var rotateRight = function(head, k) {
    if (!head || !head.next || k === 0) return head;
    
    let tail = head, n = 1;
    while (tail.next) {
        tail = tail.next;
        n++;
    }
    
    k %= n;
    if (k === 0) return head;
    
    tail.next = head;
    for (let i = 0; i < n - k; i++) {
        tail = tail.next;
    }
    
    let newHead = tail.next;
    tail.next = null;
    return newHead;
};
