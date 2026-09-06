class Solution {
public:
    int minSteps(int n) {
        if (n == 1) {
            return 0; // Base case: 1 'A' already on screen, 0 operations needed.
        }

        int total_operations = 0;
        // Iterate from 2 up to sqrt(n) to find prime factors
        for (int i = 2; i * i <= n; ++i) {
            // While i divides n, it's a prime factor
            while (n % i == 0) {
                total_operations += i; // Add the prime factor to total operations
                n /= i;               // Reduce n by dividing out this factor
            }
        }

        // If n is still greater than 1 after the loop, it means the remaining n
        // is a prime factor itself (and must be > sqrt(original n)).
        if (n > 1) {
            total_operations += n;
        }

        return total_operations;
    }
};
