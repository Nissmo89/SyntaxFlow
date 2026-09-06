var deleteDuplicates = function(head) {
    let list = head;
    while (list !== null) {
        if (list.next === null) {
            break;
        }
        if (list.val === list.next.val) {
            list.next = list.next.next;
        } else {
            list = list.next;
        }
    }
    return head;
};
