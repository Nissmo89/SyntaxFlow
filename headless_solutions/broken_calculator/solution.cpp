class Solution {
public:
    int brokenCalc(int startValue, int target) {
        int operations = 0;
        
        // Work backwards: reduce target to startValue
        while (target > startValue) {
            if (target % 2 == 1) {
                // target is odd: reverse of subtract 1 is add 1
                target += 1;
            } else {
                // target is even: reverse of multiply by 2 is divide by 2
                target /= 2;
            }
            operations++;
        }
        
        // Now target <= startValue, we need (startValue - target) decrements
        // (equivalently, (startValue - target) additions in reverse)
        return operations + (startValue - target);
    }
};
