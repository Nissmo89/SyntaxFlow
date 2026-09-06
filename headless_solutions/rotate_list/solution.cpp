class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;
        
        ListNode* tail = head;
        int n = 1;
        while (tail->next) {
            tail = tail->next;
            n++;
        }
        
        k %= n;
        if (k == 0) return head;
        
        tail->next = head; // Make it circular
        int stepsToNewTail = n - k;
        for (int i = 0; i < stepsToNewTail; ++i) {
            tail = tail->next;
        }
        
        ListNode* newHead = tail->next;
        tail->next = nullptr;
        return newHead;
    }
};
