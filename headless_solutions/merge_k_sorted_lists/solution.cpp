#include <vector>
#include <queue>

class Solution {
public:
    ListNode* mergeKLists(std::vector<ListNode*>& lists) {
        // Custom comparator for min-heap ordered by ListNode value
        auto comp = [](ListNode* a, ListNode* b) {
            return a->val > b->val;
        };
        std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(comp)> pq(comp);

        // Push initial head of each list into the heap
        for (ListNode* head : lists) {
            if (head) {
                pq.push(head);
            }
        }

        ListNode dummy(0);
        ListNode* tail = &dummy;

        // Process elements in sorted order
        while (!pq.empty()) {
            ListNode* minNode = pq.top();
            pq.pop();

            tail->next = minNode;
            tail = tail->next;

            if (minNode->next) {
                pq.push(minNode->next);
            }
        }

        return dummy.next;
    }
};
