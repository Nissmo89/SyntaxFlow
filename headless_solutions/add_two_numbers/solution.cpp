/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        // Create a dummy head node for the result list.
        // This simplifies handling the first node and avoids special checks.
        ListNode* dummyHead = new ListNode(0);
        // 'current' pointer will traverse and build the new list.
        ListNode* current = dummyHead;
        // 'carry' variable to store any carry-over from digit sums.
        int carry = 0;

        // Loop until both lists are exhausted AND there's no carry.
        while (l1 != nullptr || l2 != nullptr || carry != 0) {
            // Get the digit value from l1. If l1 is null, consider it as 0.
            int digit1 = (l1 != nullptr) ? l1->val : 0;
            // Get the digit value from l2. If l2 is null, consider it as 0.
            int digit2 = (l2 != nullptr) ? l2->val : 0;

            // Calculate the sum of current digits and the carry.
            int sum = digit1 + digit2 + carry;

            // The new digit for the result list is the unit digit of the sum.
            int newDigit = sum % 10;
            // The new carry for the next iteration is the tens digit of the sum.
            carry = sum / 10;

            // Create a new node with the calculated digit and append it to the result list.
            current->next = new ListNode(newDigit);
            // Move the 'current' pointer to the newly added node.
            current = current->next;

            // Move to the next nodes in l1 and l2, if they exist.
            if (l1 != nullptr) {
                l1 = l1->next;
            }
            if (l2 != nullptr) {
                l2 = l2->next;
            }
        }

        // The result list starts from dummyHead->next (skipping the dummy node itself).
        ListNode* result = dummyHead->next;
        // Delete the dummy head to prevent memory leaks.
        delete dummyHead; 
        return result;
    }
};
