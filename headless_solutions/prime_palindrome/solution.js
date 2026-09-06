var primePalindrome = function(n) {
    const isPrime = (num) => {
        if (num < 2) return false;
        for (let i = 2; i * i <= num; i++)
            if (num % i === 0) return false;
        return true;
    };

    if (n >= 8 && n <= 11) return 11;
    for (let len = 1; len <= 9; len++) {
        if (len % 2 === 0 && len !== 2) continue;
        let start = Math.pow(10, Math.floor((len - 1) / 2));
        let end = Math.pow(10, Math.ceil(len / 2));
        for (let i = start; i < end; i++) {
            let s = i.toString();
            let pal = s + s.split('').reverse().slice(len % 2).join('');
            let val = parseInt(pal);
            if (val >= n && isPrime(val)) return val;
        }
    }
};
