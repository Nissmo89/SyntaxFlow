/**
 * Merges k sorted linked lists using Divide and Conquer.
 */
var mergeKLists = function(lists) {
    if (!lists || lists.length === 0) return null;

    let interval = 1;
    while (interval < lists.length) {
        for (let i = 0; i < lists.length - interval; i += interval * 2) {
            lists[i] = mergeTwoLists(lists[i], lists[i + interval]);
        }
        interval *= 2;
    }

    return lists[0];
};

/**
 * Helper function to merge two sorted linked lists iteratively.
 */
function mergeTwoLists(l1, l2) {
    let dummy = new ListNode(0);
    let tail = dummy;

    while (l1 !== null && l2 !== null) {
        if (l1.val <= l2.val) {
            tail.next = l1;
            l1 = l1.next;
        } else {
            tail.next = l2;
            l2 = l2.next;
        }
        tail = tail.next;
    }

    tail.next = (l1 !== null) ? l1 : l2;
    return dummy.next;
}
