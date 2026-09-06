/**
 * @param {string} s
 * @param {string} t
 * @return {boolean}
 */
var isRationalEqual = function(s, t) {
    
    // Helper function to compute GCD
    function gcd(a, b) {
        a = Math.abs(a);
        b = Math.abs(b);
        while (b) {
            [a, b] = [b, a % b];
        }
        return a;
    }

    // Helper function to parse a rational number string into a simplified fraction [numerator, denominator]
    function parseRational(numStr) {
        let integerPart = 0;
        let nonRepeatingPart = 0;
        let repeatingPart = 0;
        let nonRepeatingLen = 0;
        let repeatingLen = 0;

        const dotPos = numStr.indexOf('.');
        const parenOpenPos = numStr.indexOf('(');
        const parenClosePos = numStr.indexOf(')');

        // Extract Integer Part
        if (dotPos === -1) {
            integerPart = parseInt(numStr);
        } else {
            integerPart = parseInt(numStr.substring(0, dotPos));
            if (parenOpenPos === -1) {
                // No repeating part, only non-repeating
                const nonRepeatingStr = numStr.substring(dotPos + 1);
                if (nonRepeatingStr.length > 0) {
                    nonRepeatingPart = parseInt(nonRepeatingStr);
                    nonRepeatingLen = nonRepeatingStr.length;
                }
            } else {
                // Has repeating part
                const nonRepeatingStr = numStr.substring(dotPos + 1, parenOpenPos);
                if (nonRepeatingStr.length > 0) {
                    nonRepeatingPart = parseInt(nonRepeatingStr);
                    nonRepeatingLen = nonRepeatingStr.length;
                }

                const repeatingStr = numStr.substring(parenOpenPos + 1, parenClosePos);
                if (repeatingStr.length > 0) {
                    repeatingPart = parseInt(repeatingStr);
                    repeatingLen = repeatingStr.length;
                }
            }
        }

        // Calculate the fraction
        // Use BigInt for potentially large numbers to avoid precision issues and overflow
        let num = 0n;
        let den = 1n;

        if (repeatingLen === 0) {
            // Case: Integer or Integer.NonRepeating
            num = BigInt(integerPart) * (10n ** BigInt(nonRepeatingLen)) + BigInt(nonRepeatingPart);
            den = 10n ** BigInt(nonRepeatingLen);
        } else {
            // Case: Integer.NonRepeating(Repeating)
            // Value = Integer + NonRepeating / 10^L_N + Repeating / (10^L_N * (10^L_R - 1))
            // Combine:
            // Numerator = I * 10^L_N * (10^L_R - 1) + N * (10^L_R - 1) + R
            // Denominator = 10^L_N * (10^L_R - 1)
            
            const repDen = (10n ** BigInt(repeatingLen)) - 1n; // 10^L_R - 1
            const commonDen = (10n ** BigInt(nonRepeatingLen)) * repDen; // 10^L_N * (10^L_R - 1)
            
            num = BigInt(integerPart) * commonDen + BigInt(nonRepeatingPart) * repDen + BigInt(repeatingPart);
            den = commonDen;
        }

        // Simplify the fraction
        if (num === 0n) {
            return [0n, 1n];
        }
        
        const commonDivisor = BigInt(gcd(Number(num), Number(den))); // Convert to Number for gcd helper, assuming it fits
        // Note: For extremely large numbers that exceed Number.MAX_SAFE_INTEGER, a BigInt GCD would be needed.
        // Given problem constraints, intermediate values should fit within standard Number for GCD calculation.
        // If not, a BigInt GCD implementation would be required.
        
        return [num / commonDivisor, den / commonDivisor];
    }

    const s_frac = parseRational(s);
    const t_frac = parseRational(t);

    return s_frac[0] === t_frac[0] && s_frac[1] === t_frac[1];
};
