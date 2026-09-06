var removeNthFromEnd = function(head, n) {
    let dummy = new ListNode(0, head);
    let fast = dummy, slow = dummy;
    
    for (let i = 0; i <= n; i++) fast = fast.next;
    
    while (fast !== null) {
        fast = fast.next;
        slow = slow.next;
    }
    
    slow.next = slow.next.next;
    return dummy.next;
};
