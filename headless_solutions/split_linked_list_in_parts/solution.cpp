class Solution {
public:
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        int n = 0;
        ListNode* curr = head;
        while (curr) { n++; curr = curr->next; }
        
        int width = n / k, rem = n % k;
        vector<ListNode*> res(k, nullptr);
        curr = head;
        for (int i = 0; i < k; ++i) {
            if (!curr) break;
            res[i] = curr;
            int partSize = width + (rem-- > 0 ? 1 : 0);
            for (int j = 0; j < partSize - 1; ++j) curr = curr->next;
            ListNode* nextPart = curr->next;
            curr->next = nullptr;
            curr = nextPart;
        }
        return res;
    }
};
