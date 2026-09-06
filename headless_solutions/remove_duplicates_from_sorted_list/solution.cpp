class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* list = head;
        while (list != nullptr) {
            if (list->next == nullptr) {
                break;
            }
            if (list->val == list->next->val) {
                list->next = list->next->next;
            } else {
                list = list->next;
            }
        }
        return head;
    }
};
