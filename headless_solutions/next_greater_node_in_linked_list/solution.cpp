class Solution {
public:
    vector<int> nextLargerNodes(ListNode* head) {
        vector<int> vals;
        for (ListNode* curr = head; curr; curr = curr->next) vals.push_back(curr->val);
        
        vector<int> res(vals.size(), 0);
        vector<int> stack; // Stores indices
        for (int i = 0; i < vals.size(); ++i) {
            while (!stack.empty() && vals[stack.back()] < vals[i]) {
                res[stack.back()] = vals[i];
                stack.pop_back();
            }
            stack.push_back(i);
        }
        return res;
    }
};
