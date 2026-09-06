var reverseBetween = function(head, left, right) {
    if (!head || left === right) return head;
    let dummy = new ListNode(0, head);
    let prev = dummy;
    for (let i = 0; i < left - 1; i++) prev = prev.next;
    
    let start = prev.next;
    let then = start.next;
    
    for (let i = 0; i < right - left; i++) {
        start.next = then.next;
        then.next = prev.next;
        prev.next = then;
        then = start.next;
    }
    return dummy.next;
};
