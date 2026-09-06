var permute = function(nums) {
    const res = [];
    const used = new Array(nums.length).fill(false);
    
    const backtrack = (curr) => {
        if (curr.length === nums.length) {
            res.push([...curr]);
            return;
        }
        for (let i = 0; i < nums.length; i++) {
            if (!used[i]) {
                used[i] = true;
                curr.push(nums[i]);
                backtrack(curr);
                curr.pop();
                used[i] = false;
            }
        }
    };
    backtrack([]);
    return res;
};
