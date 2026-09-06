class Solution {
    isPalindrome(s) {
        let left = 0;
        let right = s.length - 1;
        
        while (left < right) {
            if (!/^[a-zA-Z0-9]$/.test(s[left])) {
                left++;
                continue;
            }
            if (!/^[a-zA-Z0-9]$/.test(s[right])) {
                right--;
                continue;
            }
            if (s[left].toLowerCase() !== s[right].toLowerCase()) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }
}
