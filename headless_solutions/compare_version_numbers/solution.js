/**
 * @param {string} version1
 * @param {string} version2
 * @return {number}
 */
var compareVersion = function(version1, version2) {
    let p1 = 0; // Pointer for version1
    let p2 = 0; // Pointer for version2
    const n1 = version1.length;
    const n2 = version2.length;

    // Iterate as long as there are characters to process in either string
    while (p1 < n1 || p2 < n2) {
        let num1 = 0;
        let num2 = 0;

        // Parse the current revision from version1
        while (p1 < n1 && version1[p1] !== '.') {
            num1 = num1 * 10 + parseInt(version1[p1]);
            p1++;
        }

        // Parse the current revision from version2
        while (p2 < n2 && version2[p2] !== '.') {
            num2 = num2 * 10 + parseInt(version2[p2]);
            p2++;
        }

        // Compare the parsed revisions
        if (num1 > num2) {
            return 1;
        }
        if (num1 < num2) {
            return -1;
        }

        // Move past the dot if it exists
        if (p1 < n1) {
            p1++; // Skip the '.'
        }
        if (p2 < n2) {
            p2++; // Skip the '.'
        }
    }

    // If all revisions were equal
    return 0;
};
