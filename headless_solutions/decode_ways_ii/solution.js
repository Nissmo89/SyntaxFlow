var numDecodings = function(s) {
    const MOD = 1e9 + 7;
    let prev2 = 1, prev1 = s[0] === '*' ? 9 : (s[0] === '0' ? 0 : 1);
    
    for (let i = 1; i < s.length; i++) {
        let curr = 0;
        if (s[i] === '*') curr = (prev1 * 9) % MOD;
        else if (s[i] !== '0') curr = prev1;
        
        if (s[i-1] === '*' && s[i] === '*') curr = (curr + prev2 * 15) % MOD;
        else if (s[i-1] === '*') curr = (curr + prev2 * (s[i] <= '6' ? 2 : 1)) % MOD;
        else if (s[i] === '*') curr = (curr + prev2 * (s[i-1] === '1' ? 9 : (s[i-1] === '2' ? 6 : 0))) % MOD;
        else {
            let val = parseInt(s.substring(i-1, i+1));
            if (val >= 10 && val <= 26) curr = (curr + prev2) % MOD;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
};
