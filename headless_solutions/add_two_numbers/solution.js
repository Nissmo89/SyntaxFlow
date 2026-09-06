/**
 * Definition for singly-linked list.
 * function ListNode(val, next) {
 *     this.val = (val===undefined ? 0 : val)
 *     this.next = (next===undefined ? null : next)
 * }
 */
/**
 * @param {ListNode} l1
 * @param {ListNode} l2
 * @return {ListNode}
 */
var addTwoNumbers = function(l1, l2) {
    // Create a dummy head node for the result list.
    // This simplifies handling the first node and avoids special checks.
    let dummyHead = new ListNode(0);
    // 'current' pointer will traverse and build the new list.
    let current = dummyHead;
    // 'carry' variable to store any carry-over from digit sums.
    let carry = 0;

    // Loop until both lists are exhausted AND there's no carry.
    while (l1 !== null || l2 !== null || carry !== 0) {
        // Get the digit value from l1. If l1 is null, consider it as 0.
        let digit1 = (l1 !== null) ? l1.val : 0;
        // Get the digit value from l2. If l2 is null, consider it as 0.
        let digit2 = (l2 !== null) ? l2.val : 0;

        // Calculate the sum of current digits and the carry.
        let sum = digit1 + digit2 + carry;

        // The new digit for the result list is the unit digit of the sum.
        let newDigit = sum % 10;
        // The new carry for the next iteration is the tens digit of the sum.
        carry = Math.floor(sum / 10); // Use Math.floor for integer division

        // Create a new node with the calculated digit and append it to the result list.
        current.next = new ListNode(newDigit);
        // Move the 'current' pointer to the newly added node.
        current = current.next;

        // Move to the next nodes in l1 and l2, if they exist.
        if (l1 !== null) {
            l1 = l1.next;
        }
        if (l2 !== null) {
            l2 = l2.next;
        }
    }

    // The result list starts from dummyHead.next (skipping the dummy node itself).
    return dummyHead.next;
};
