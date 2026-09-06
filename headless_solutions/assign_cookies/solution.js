/**
 * @param {number[]} g
 * @param {number[]} s
 * @return {number}
 */
var findContentChildren = function(g, s) {
    // Sort greed factors in ascending order
    g.sort((a, b) => a - b);
    // Sort cookie sizes in ascending order
    s.sort((a, b) => a - b);

    let childIdx = 0; // Pointer for children's greed factors
    let cookieIdx = 0; // Pointer for cookie sizes
    let contentChildrenCount = 0; // Counter for content children

    // Iterate while there are children and cookies left
    while (childIdx < g.length && cookieIdx < s.length) {
        // If the current cookie can satisfy the current child
        if (s[cookieIdx] >= g[childIdx]) {
            contentChildrenCount++; // Make this child content
            childIdx++; // Move to the next child
            cookieIdx++; // Move to the next cookie (it's been used)
        } else {
            // The current cookie is too small for the current child.
            // Since children are sorted by greed, this cookie will also be too small
            // for any subsequent child. So, discard this cookie and try the next one.
            cookieIdx++;
        }
    }

    return contentChildrenCount;
};
