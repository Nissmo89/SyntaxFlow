#include <vector>
#include <algorithm> // Required for std::sort

class Solution {
public:
    int findContentChildren(std::vector<int>& g, std::vector<int>& s) {
        // Sort greed factors in ascending order
        std::sort(g.begin(), g.end());
        // Sort cookie sizes in ascending order
        std::sort(s.begin(), s.end());

        int child_idx = 0; // Pointer for children's greed factors
        int cookie_idx = 0; // Pointer for cookie sizes
        int content_children_count = 0; // Counter for content children

        // Iterate while there are children and cookies left
        while (child_idx < g.size() && cookie_idx < s.size()) {
            // If the current cookie can satisfy the current child
            if (s[cookie_idx] >= g[child_idx]) {
                content_children_count++; // Make this child content
                child_idx++; // Move to the next child
                cookie_idx++; // Move to the next cookie (it's been used)
            } else {
                // The current cookie is too small for the current child.
                // Since children are sorted by greed, this cookie will also be too small
                // for any subsequent child. So, discard this cookie and try the next one.
                cookie_idx++;
            }
        }

        return content_children_count;
    }
};
