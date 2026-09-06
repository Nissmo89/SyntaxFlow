class Solution {
public:
    int numComponents(ListNode* head, vector<int>& nums) {
        unordered_set<int> setNums(nums.begin(), nums.end());
        int count = 0;
        while (head != nullptr) {
            // If current node is in nums and (next is null or next is not in nums)
            if (setNums.count(head->val) && (head->next == nullptr || !setNums.count(head->next->val))) {
                count++;
            }
            head = head->next;
        }
        return count;
    }
};
