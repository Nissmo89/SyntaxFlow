class Solution {
public:
    ListNode* insertionSortList(ListNode* head) {
        if (!head || !head->next) return head;
        ListNode* dummy = new ListNode(0, head);
        ListNode* curr = head;
        while (curr->next) {
            if (curr->val <= curr->next->val) {
                curr = curr->next;
            } else {
                ListNode* temp = curr->next;
                curr->next = temp->next;
                ListNode* prev = dummy;
                while (prev->next->val <= temp->val) prev = prev->next;
                temp->next = prev->next;
                prev->next = temp;
            }
        }
        return dummy->next;
    }
};
