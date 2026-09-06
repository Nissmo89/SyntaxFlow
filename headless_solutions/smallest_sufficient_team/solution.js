var smallestSufficientTeam = function(req_skills, people) {
    const n = req_skills.length;
    const skillMap = new Map(req_skills.map((s, i) => [s, i]));
    const dp = new Array(1 << n).fill(null);
    dp[0] = [];

    for (let i = 0; i < people.length; i++) {
        let personMask = 0;
        for (const s of people[i]) personMask |= (1 << skillMap.get(s));
        for (let mask = 0; mask < (1 << n); mask++) {
            if (dp[mask] === null) continue;
            const nextMask = mask | personMask;
            if (dp[nextMask] === null || dp[nextMask].length > dp[mask].length + 1) {
                dp[nextMask] = [...dp[mask], i];
            }
        }
    }
    return dp[(1 << n) - 1];
};
