var isPalindrome = function(head) {
    if (head === null || head.next === null) {
        return true;
    }

    let fast = head;
    let slow = head;

    // Find the middle of the linked list
    while (fast.next !== null && fast.next.next !== null) {
        fast = fast.next.next;
        slow = slow.next;
    }

    // Reverse the second half of the linked list
    let prev = null;
    let curr = slow.next;
    while (curr !== null) {
        let next_node = curr.next;
        curr.next = prev;
        prev = curr;
        curr = next_node;
    }

    // Compare the node values from the beginning and the end of the linked list
    while (prev !== null) {
        if (head.val !== prev.val) {
            return false;
        }
        head = head.next;
        prev = prev.next;
    }

    return true;
};
