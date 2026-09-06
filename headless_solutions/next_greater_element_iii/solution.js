var nextGreaterElement = function(n) {
    let digits = n.toString().split('');
    let i = digits.length - 1;
    while (i > 0 && digits[i - 1] >= digits[i]) i--;
    if (i === 0) return -1;
    
    let j = digits.length - 1;
    while (digits[j] <= digits[i - 1]) j--;
    
    [digits[i - 1], digits[j]] = [digits[j], digits[i - 1]];
    let suffix = digits.slice(i).reverse();
    digits = digits.slice(0, i).concat(suffix);
    
    let res = parseInt(digits.join(''));
    return res > 2147483647 ? -1 : res;
};
