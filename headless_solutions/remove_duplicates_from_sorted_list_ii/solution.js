var deleteDuplicates = function(head) {
    let dummy = new ListNode(0, head);
    let prev = dummy;
    let curr = head;
    
    while (curr !== null) {
        if (curr.next !== null && curr.val === curr.next.val) {
            while (curr.next !== null && curr.val === curr.next.val) {
                curr = curr.next;
            }
            prev.next = curr.next;
        } else {
            prev = prev.next;
        }
        curr = curr.next;
    }
    return dummy.next;
};
