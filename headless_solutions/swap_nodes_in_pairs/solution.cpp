class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if (!head || !head->next) return head;
        
        ListNode* first = head;
        ListNode* second = head->next;
        
        // Recursion: first node points to the result of the rest
        first->next = swapPairs(second->next);
        // Second node points to first
        second->next = first;
        
        return second;
    }
};
