var majorityElement = function(nums) {
    let cand1 = null, cand2 = null, count1 = 0, count2 = 0;
    for (let n of nums) {
        if (n === cand1) count1++;
        else if (n === cand2) count2++;
        else if (count1 === 0) { cand1 = n; count1 = 1; }
        else if (count2 === 0) { cand2 = n; count2 = 1; }
        else { count1--; count2--; }
    }
    count1 = count2 = 0;
    for (let n of nums) {
        if (n === cand1) count1++;
        else if (n === cand2) count2++;
    }
    let res = [];
    if (count1 > nums.length / 3) res.push(cand1);
    if (count2 > nums.length / 3) res.push(cand2);
    return res;
};
