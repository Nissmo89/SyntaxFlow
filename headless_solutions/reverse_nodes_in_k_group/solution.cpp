class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0, head);
        ListNode *prevGroup = &dummy, *curr = head;
        
        while (true) {
            ListNode* kth = prevGroup;
            for (int i = 0; i < k && kth; ++i) kth = kth->next;
            if (!kth) break;
            
            ListNode* nextGroup = kth->next;
            ListNode* prev = nextGroup;
            ListNode* currNode = prevGroup->next;
            
            for (int i = 0; i < k; ++i) {
                ListNode* tmp = currNode->next;
                currNode->next = prev;
                prev = currNode;
                currNode = tmp;
            }
            
            ListNode* tmp = prevGroup->next;
            prevGroup->next = kth;
            prevGroup = tmp;
        }
        return dummy.next;
    }
};
