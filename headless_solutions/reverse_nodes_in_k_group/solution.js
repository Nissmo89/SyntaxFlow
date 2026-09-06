var reverseKGroup = function(head, k) {
    let dummy = new ListNode(0, head);
    let prevGroup = dummy;
    
    while (true) {
        let kth = prevGroup;
        for (let i = 0; i < k && kth; i++) kth = kth.next;
        if (!kth) break;
        
        let nextGroup = kth.next;
        let prev = nextGroup, curr = prevGroup.next;
        for (let i = 0; i < k; i++) {
            let tmp = curr.next;
            curr.next = prev;
            prev = curr;
            curr = tmp;
        }
        let tmp = prevGroup.next;
        prevGroup.next = kth;
        prevGroup = tmp;
    }
    return dummy.next;
};
