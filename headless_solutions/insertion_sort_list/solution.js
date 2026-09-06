var insertionSortList = function(head) {
    let dummy = new ListNode(0, head);
    let curr = head;
    while (curr && curr.next) {
        if (curr.val <= curr.next.val) {
            curr = curr.next;
        } else {
            let temp = curr.next;
            curr.next = temp.next;
            let prev = dummy;
            while (prev.next.val <= temp.val) prev = prev.next;
            temp.next = prev.next;
            prev.next = temp;
        }
    }
    return dummy.next;
};
