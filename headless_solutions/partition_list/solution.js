var partition = function(head, x) {
    let beforeHead = new ListNode(0), afterHead = new ListNode(0);
    let before = beforeHead, after = afterHead;
    
    while (head) {
        if (head.val < x) {
            before.next = head;
            before = before.next;
        } else {
            after.next = head;
            after = after.next;
        }
        head = head.next;
    }
    
    after.next = null;
    before.next = afterHead.next;
    return beforeHead.next;
};
