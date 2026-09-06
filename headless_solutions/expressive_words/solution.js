var expressiveWords = function(s, words) {
    const getGroups = (str) => {
        let groups = [];
        for (let i = 0; i < str.length; ) {
            let j = i;
            while (j < str.length && str[j] === str[i]) j++;
            groups.push([str[i], j - i]);
            i = j;
        }
        return groups;
    };

    const sGroups = getGroups(s);
    let count = 0;
    for (const word of words) {
        const wGroups = getGroups(word);
        if (sGroups.length !== wGroups.length) continue;
        if (sGroups.every((g, idx) => g[0] === wGroups[idx][0] && 
            g[1] >= wGroups[idx][1] && (g[1] === wGroups[idx][1] || g[1] >= 3))) {
            count++;
        }
    }
    return count;
};
