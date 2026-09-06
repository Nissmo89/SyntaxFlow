class Solution {
public:
    bool isValidSerialization(string preorder) {
        int slots = 1;
        stringstream ss(preorder);
        string node;
        while (getline(ss, node, ',')) {
            if (slots <= 0) return false;
            if (node == "#") {
                slots--;
            } else {
                slots++; // -1 (consume) + 2 (add) = +1
            }
        }
        return slots == 0;
    }
};
