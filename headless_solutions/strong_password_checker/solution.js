var strongPasswordChecker = function(password) {
    let n = password.length;
    let missing = (!/[a-z]/.test(password)) + (!/[A-Z]/.test(password)) + (!/[0-9]/.test(password));
    let counts = [], i = 0;
    while (i < n) {
        let j = i;
        while (j < n && password[j] === password[i]) j++;
        if (j - i >= 3) counts.push(j - i);
        i = j;
    }
    if (n < 6) return Math.max(missing, 6 - n);
    let replace = counts.reduce((a, b) => a + Math.floor(b / 3), 0);
    if (n <= 20) return Math.max(missing, replace);
    let del = n - 20, mod = [0, 0, 0];
    counts.forEach(c => mod[c % 3]++);
    for (let i = 1; i <= 2; i++) {
        let take = Math.min(mod[i], Math.floor(del / i));
        del -= take * i;
        replace -= take;
    }
    replace -= Math.floor(del / 3);
    return (n - 20) + Math.max(missing, replace);
};
