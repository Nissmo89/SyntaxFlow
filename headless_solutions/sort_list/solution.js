var sortList = function(head) {
    if (!head || !head.next) return head;
    
    let prev = null, slow = head, fast = head;
    while (fast && fast.next) {
        prev = slow;
        slow = slow.next;
        fast = fast.next.next;
    }
    prev.next = null;
    
    return merge(sortList(head), sortList(slow));
};

function merge(l1, l2) {
    let dummy = new ListNode(0), tail = dummy;
    while (l1 && l2) {
        if (l1.val < l2.val) { tail.next = l1; l1 = l1.next; }
        else { tail.next = l2; l2 = l2.next; }
        tail = tail.next;
    }
    tail.next = l1 || l2;
    return dummy.next;
}
