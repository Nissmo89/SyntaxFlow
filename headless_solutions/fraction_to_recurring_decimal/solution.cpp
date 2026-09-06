#include <string>
#include <unordered_map>
#include <cmath>

class Solution {
public:
    string fractionToDecimal(int numerator, int denominator) {
        if (numerator == 0) return "0";
        string res = "";
        if ((numerator < 0) ^ (denominator < 0)) res += '-';
        
        long n = abs((long)numerator);
        long d = abs((long)denominator);
        
        res += to_string(n / d);
        long rem = n % d;
        if (rem == 0) return res;
        
        res += '.';
        unordered_map<long, int> map;
        while (rem != 0) {
            if (map.find(rem) != map.end()) {
                res.insert(map[rem], "(");
                res += ')';
                break;
            }
            map[rem] = res.length();
            rem *= 10;
            res += to_string(rem / d);
            rem %= d;
        }
        return res;
    }
};
